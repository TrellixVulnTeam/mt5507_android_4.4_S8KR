// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/renderer/gpu/render_widget_compositor.h"

#include <limits>
#include <string>

#if defined(OS_ANDROID)
#include "base/android/sys_utils.h"
#endif

#include "base/command_line.h"
#include "base/logging.h"
#include "base/strings/string_number_conversions.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "base/values.h"
#include "cc/base/latency_info_swap_promise.h"
#include "cc/base/latency_info_swap_promise_monitor.h"
#include "cc/base/switches.h"
#include "cc/debug/layer_tree_debug_state.h"
#include "cc/debug/micro_benchmark.h"
#include "cc/layers/layer.h"
#include "cc/trees/layer_tree_host.h"
#include "content/common/gpu/client/context_provider_command_buffer.h"
#include "content/public/common/content_switches.h"
#include "content/renderer/input/input_handler_manager.h"
#include "content/renderer/render_thread_impl.h"
#include "third_party/WebKit/public/platform/WebSize.h"
#include "third_party/WebKit/public/web/WebWidget.h"
#include "ui/gl/gl_switches.h"
#include "webkit/renderer/compositor_bindings/web_layer_impl.h"

/// M: add debug info flags from properties
#include <cutils/properties.h>

namespace base {
class Value;
}

namespace cc {
class Layer;
}

using blink::WebFloatPoint;
using blink::WebSize;
using blink::WebRect;

namespace content {
namespace {

/// M: added debug setting can enable from properties @{
enum DebugInfoFlag {
  ShowDebugBorder = 0x01,
  ShowFpsCounter = ShowDebugBorder << 1,
  ShowPaintRects = ShowFpsCounter << 1,
  ShowPropertyChangedRects = ShowPaintRects << 1,
  ShowSurfaceDamageRects = ShowPropertyChangedRects << 1,
  ShowScreenSpaceRects = ShowSurfaceDamageRects << 1,
  ShowReplicaScreenSpaceRects = ShowScreenSpaceRects << 1,
  ShowOccludingRects = ShowReplicaScreenSpaceRects << 1,
  ShowNonOccludingRects = ShowOccludingRects << 1,
};
/// @}


bool GetSwitchValueAsInt(
    const CommandLine& command_line,
    const std::string& switch_string,
    int min_value,
    int max_value,
    int* result) {
  std::string string_value = command_line.GetSwitchValueASCII(switch_string);
  int int_value;
  if (base::StringToInt(string_value, &int_value) &&
      int_value >= min_value && int_value <= max_value) {
    *result = int_value;
    return true;
  } else {
    LOG(WARNING) << "Failed to parse switch " << switch_string  << ": " <<
        string_value;
    return false;
  }
}

}  // namespace

// static
scoped_ptr<RenderWidgetCompositor> RenderWidgetCompositor::Create(
    RenderWidget* widget,
    bool threaded) {
  scoped_ptr<RenderWidgetCompositor> compositor(
      new RenderWidgetCompositor(widget, threaded));

  CommandLine* cmd = CommandLine::ForCurrentProcess();

  cc::LayerTreeSettings settings;

  // For web contents, layer transforms should scale up the contents of layers
  // to keep content always crisp when possible.
  settings.layer_transforms_should_scale_layer_contents = true;

  settings.throttle_frame_production =
      !cmd->HasSwitch(switches::kDisableGpuVsync);
  settings.begin_impl_frame_scheduling_enabled =
      cmd->HasSwitch(switches::kEnableBeginFrameScheduling);
  settings.deadline_scheduling_enabled =
      cmd->HasSwitch(switches::kEnableDeadlineScheduling) &&
      !cmd->HasSwitch(switches::kDisableDeadlineScheduling);
  settings.using_synchronous_renderer_compositor =
      widget->UsingSynchronousRendererCompositor();
  settings.per_tile_painting_enabled =
      cmd->HasSwitch(cc::switches::kEnablePerTilePainting);
  settings.accelerated_animation_enabled =
      !cmd->HasSwitch(cc::switches::kDisableThreadedAnimation);
  settings.touch_hit_testing =
      !cmd->HasSwitch(cc::switches::kDisableCompositorTouchHitTesting);

  int default_tile_width = settings.default_tile_size.width();
  if (cmd->HasSwitch(switches::kDefaultTileWidth)) {
    GetSwitchValueAsInt(*cmd, switches::kDefaultTileWidth, 1,
                        std::numeric_limits<int>::max(), &default_tile_width);
  }
  int default_tile_height = settings.default_tile_size.height();
  if (cmd->HasSwitch(switches::kDefaultTileHeight)) {
    GetSwitchValueAsInt(*cmd, switches::kDefaultTileHeight, 1,
                        std::numeric_limits<int>::max(), &default_tile_height);
  }
  settings.default_tile_size = gfx::Size(default_tile_width,
                                         default_tile_height);

  int max_untiled_layer_width = settings.max_untiled_layer_size.width();
  if (cmd->HasSwitch(switches::kMaxUntiledLayerWidth)) {
    GetSwitchValueAsInt(*cmd, switches::kMaxUntiledLayerWidth, 1,
                        std::numeric_limits<int>::max(),
                        &max_untiled_layer_width);
  }
  int max_untiled_layer_height = settings.max_untiled_layer_size.height();
  if (cmd->HasSwitch(switches::kMaxUntiledLayerHeight)) {
    GetSwitchValueAsInt(*cmd, switches::kMaxUntiledLayerHeight, 1,
                        std::numeric_limits<int>::max(),
                        &max_untiled_layer_height);
  }

  settings.max_untiled_layer_size = gfx::Size(max_untiled_layer_width,
                                           max_untiled_layer_height);

  settings.impl_side_painting = cc::switches::IsImplSidePaintingEnabled();
  settings.gpu_rasterization = cc::switches::IsGPURasterizationEnabled();

  settings.calculate_top_controls_position =
      cmd->HasSwitch(cc::switches::kEnableTopControlsPositionCalculation);
  if (cmd->HasSwitch(cc::switches::kTopControlsHeight)) {
    std::string controls_height_str =
        cmd->GetSwitchValueASCII(cc::switches::kTopControlsHeight);
    double controls_height;
    if (base::StringToDouble(controls_height_str, &controls_height) &&
        controls_height > 0)
      settings.top_controls_height = controls_height;
  }

  if (settings.calculate_top_controls_position &&
      settings.top_controls_height <= 0) {
    DCHECK(false)
        << "Top controls repositioning enabled without valid height set.";
    settings.calculate_top_controls_position = false;
  }

  if (cmd->HasSwitch(cc::switches::kTopControlsShowThreshold)) {
      std::string top_threshold_str =
          cmd->GetSwitchValueASCII(cc::switches::kTopControlsShowThreshold);
      double show_threshold;
      if (base::StringToDouble(top_threshold_str, &show_threshold) &&
          show_threshold >= 0.f && show_threshold <= 1.f)
        settings.top_controls_show_threshold = show_threshold;
  }

  if (cmd->HasSwitch(cc::switches::kTopControlsHideThreshold)) {
      std::string top_threshold_str =
          cmd->GetSwitchValueASCII(cc::switches::kTopControlsHideThreshold);
      double hide_threshold;
      if (base::StringToDouble(top_threshold_str, &hide_threshold) &&
          hide_threshold >= 0.f && hide_threshold <= 1.f)
        settings.top_controls_hide_threshold = hide_threshold;
  }

  settings.partial_swap_enabled = widget->AllowPartialSwap() &&
      cmd->HasSwitch(cc::switches::kEnablePartialSwap);
  settings.background_color_instead_of_checkerboard =
      cmd->HasSwitch(cc::switches::kBackgroundColorInsteadOfCheckerboard);
  settings.show_overdraw_in_tracing =
      cmd->HasSwitch(cc::switches::kTraceOverdraw);
  settings.can_use_lcd_text = cc::switches::IsLCDTextEnabled();
  settings.use_pinch_virtual_viewport =
      cmd->HasSwitch(cc::switches::kEnablePinchVirtualViewport);
  settings.allow_antialiasing &=
      !cmd->HasSwitch(cc::switches::kDisableCompositedAntialiasing);

  // These flags should be mirrored by UI versions in ui/compositor/.
  settings.initial_debug_state.show_debug_borders =
      cmd->HasSwitch(cc::switches::kShowCompositedLayerBorders);
  settings.initial_debug_state.show_fps_counter =
      cmd->HasSwitch(cc::switches::kShowFPSCounter);
  settings.initial_debug_state.show_layer_animation_bounds_rects =
      cmd->HasSwitch(cc::switches::kShowLayerAnimationBounds);
  settings.initial_debug_state.show_paint_rects =
      cmd->HasSwitch(switches::kShowPaintRects);
  settings.initial_debug_state.show_property_changed_rects =
      cmd->HasSwitch(cc::switches::kShowPropertyChangedRects);
  settings.initial_debug_state.show_surface_damage_rects =
      cmd->HasSwitch(cc::switches::kShowSurfaceDamageRects);
  settings.initial_debug_state.show_screen_space_rects =
      cmd->HasSwitch(cc::switches::kShowScreenSpaceRects);
  settings.initial_debug_state.show_replica_screen_space_rects =
      cmd->HasSwitch(cc::switches::kShowReplicaScreenSpaceRects);
  settings.initial_debug_state.show_occluding_rects =
      cmd->HasSwitch(cc::switches::kShowOccludingRects);
  settings.initial_debug_state.show_non_occluding_rects =
      cmd->HasSwitch(cc::switches::kShowNonOccludingRects);

  settings.initial_debug_state.SetRecordRenderingStats(
      cmd->HasSwitch(switches::kEnableGpuBenchmarking));

  if (cmd->HasSwitch(cc::switches::kSlowDownRasterScaleFactor)) {
    const int kMinSlowDownScaleFactor = 0;
    const int kMaxSlowDownScaleFactor = INT_MAX;
    GetSwitchValueAsInt(
        *cmd,
        cc::switches::kSlowDownRasterScaleFactor,
        kMinSlowDownScaleFactor,
        kMaxSlowDownScaleFactor,
        &settings.initial_debug_state.slow_down_raster_scale_factor);
  }

  if (cmd->HasSwitch(cc::switches::kNumRasterThreads)) {
    const int kMinRasterThreads = 1;
    const int kMaxRasterThreads = 64;
    int num_raster_threads;
    if (GetSwitchValueAsInt(*cmd, cc::switches::kNumRasterThreads,
                            kMinRasterThreads, kMaxRasterThreads,
                            &num_raster_threads))
      settings.num_raster_threads = num_raster_threads;
  }

  if (cmd->HasSwitch(cc::switches::kMaxTilesForInterestArea)) {
    int max_tiles_for_interest_area;
    if (GetSwitchValueAsInt(*cmd,
                            cc::switches::kMaxTilesForInterestArea,
                            1, std::numeric_limits<int>::max(),
                            &max_tiles_for_interest_area))
      settings.max_tiles_for_interest_area = max_tiles_for_interest_area;
  }

  if (cmd->HasSwitch(cc::switches::kMaxUnusedResourceMemoryUsagePercentage)) {
    int max_unused_resource_memory_percentage;
    if (GetSwitchValueAsInt(
            *cmd,
            cc::switches::kMaxUnusedResourceMemoryUsagePercentage,
            0, 100,
            &max_unused_resource_memory_percentage)) {
      settings.max_unused_resource_memory_percentage =
          max_unused_resource_memory_percentage;
    }
  }

  settings.strict_layer_property_change_checking =
      cmd->HasSwitch(cc::switches::kStrictLayerPropertyChangeChecking);

  settings.use_map_image = cc::switches::IsMapImageEnabled();

  LOG(INFO)<<"[RenderWidgetCompositor::Create] ["<<settings.default_tile_size.width()<<","<<settings.default_tile_size.height()<<"] ["<<max_untiled_layer_width<<","
    <<max_untiled_layer_height<<"] gpuRaster=["<<settings.gpu_rasterization<<"] ["
    <<settings.num_raster_threads<<"] ["<<settings.max_tiles_for_interest_area<<"] ["
    <<settings.max_unused_resource_memory_percentage<<"] ["<<settings.use_map_image<<"]";;


#if defined(OS_ANDROID)
  // TODO(danakj): Move these to the android code.
  settings.max_partial_texture_updates = 0;
  settings.scrollbar_animator = cc::LayerTreeSettings::LinearFade;
  settings.solid_color_scrollbar_color =
      cmd->HasSwitch(switches::kHideScrollbars)
          ? SK_ColorTRANSPARENT
          : SkColorSetARGB(128, 128, 128, 128);
  settings.highp_threshold_min = 2048;
  // Android WebView handles root layer flings itself.
  settings.ignore_root_layer_flings =
      widget->UsingSynchronousRendererCompositor();
  settings.always_overscroll = widget->UsingSynchronousRendererCompositor();
  // RGBA_4444 textures are only enabled for low end devices
  // and are disabled for Android WebView as it doesn't support the format.
  settings.use_rgba_4444_textures =
      base::android::SysUtils::IsLowEndDevice() &&
      !widget->UsingSynchronousRendererCompositor() &&
      !cmd->HasSwitch(cc::switches::kDisable4444Textures);
  // Webview does not own the surface so should not clear it.
  settings.should_clear_root_render_pass =
      !widget->UsingSynchronousRendererCompositor();

/// M: add debug info flags from properties @{
  char value[PROPERTY_VALUE_MAX] = {'\0'};
  property_get("chromium.debugInfo", value, "");

  if (strlen(value) > 0) {
    int flags = strtol(value, NULL, 16);
    LOG(INFO) << "[RenderWidgetCompositor] setup debug flags=" << flags;

    settings.initial_debug_state.show_debug_borders = ((flags & ShowDebugBorder) > 0) ? true : false;
    settings.initial_debug_state.show_fps_counter = ((flags & ShowFpsCounter) > 0) ? true : false;
    settings.initial_debug_state.show_paint_rects = ((flags & ShowPaintRects) > 0) ? true : false;
    settings.initial_debug_state.show_property_changed_rects = ((flags & ShowPropertyChangedRects) > 0) ? true : false;
    settings.initial_debug_state.show_surface_damage_rects = ((flags & ShowSurfaceDamageRects) > 0) ? true : false;
    settings.initial_debug_state.show_screen_space_rects = ((flags & ShowScreenSpaceRects) > 0) ? true : false;
    settings.initial_debug_state.show_replica_screen_space_rects = ((flags & ShowReplicaScreenSpaceRects) > 0) ? true : false;
    settings.initial_debug_state.show_occluding_rects = ((flags & ShowOccludingRects) > 0) ? true : false;
    settings.initial_debug_state.show_non_occluding_rects = ((flags & ShowNonOccludingRects) > 0) ? true : false;
  }
  /// @}

#elif !defined(OS_MACOSX)
  if (cmd->HasSwitch(switches::kEnableOverlayScrollbars)) {
    settings.scrollbar_animator = cc::LayerTreeSettings::Thinning;
  }
  if (cmd->HasSwitch(cc::switches::kEnablePinchVirtualViewport) ||
      cmd->HasSwitch(switches::kEnableOverlayScrollbars)) {
    settings.solid_color_scrollbar_color = SkColorSetARGB(128, 128, 128, 128);
  }
#endif

  if (!compositor->Initialize(settings))
    return scoped_ptr<RenderWidgetCompositor>();

  return compositor.Pass();
}

RenderWidgetCompositor::RenderWidgetCompositor(RenderWidget* widget,
                                               bool threaded)
    : threaded_(threaded),
      suppress_schedule_composite_(false),
      widget_(widget) {
}

RenderWidgetCompositor::~RenderWidgetCompositor() {}

const base::WeakPtr<cc::InputHandler>&
RenderWidgetCompositor::GetInputHandler() {
  return layer_tree_host_->GetInputHandler();
}

void RenderWidgetCompositor::SetSuppressScheduleComposite(bool suppress) {
  if (suppress_schedule_composite_ == suppress)
    return;

  if (suppress)
    TRACE_EVENT_ASYNC_BEGIN0("gpu",
        "RenderWidgetCompositor::SetSuppressScheduleComposite", this);
  else
    TRACE_EVENT_ASYNC_END0("gpu",
        "RenderWidgetCompositor::SetSuppressScheduleComposite", this);
  suppress_schedule_composite_ = suppress;
}

bool RenderWidgetCompositor::BeginMainFrameRequested() const {
  return layer_tree_host_->BeginMainFrameRequested();
}

void RenderWidgetCompositor::Animate(base::TimeTicks time) {
  layer_tree_host_->UpdateClientAnimations(time);
}

void RenderWidgetCompositor::Composite(base::TimeTicks frame_begin_time) {
  layer_tree_host_->Composite(frame_begin_time);
}

void RenderWidgetCompositor::SetNeedsDisplayOnAllLayers() {
  layer_tree_host_->SetNeedsDisplayOnAllLayers();
}

void RenderWidgetCompositor::SetRasterizeOnlyVisibleContent() {
  cc::LayerTreeDebugState current = layer_tree_host_->debug_state();
  current.rasterize_only_visible_content = true;
  layer_tree_host_->SetDebugState(current);
}

void RenderWidgetCompositor::GetRenderingStats(cc::RenderingStats* stats) {
  layer_tree_host_->CollectRenderingStats(stats);
}

void RenderWidgetCompositor::UpdateTopControlsState(
    cc::TopControlsState constraints,
    cc::TopControlsState current,
    bool animate) {
  layer_tree_host_->UpdateTopControlsState(constraints,
                                           current,
                                           animate);
}

void RenderWidgetCompositor::SetOverdrawBottomHeight(
    float overdraw_bottom_height) {
  layer_tree_host_->SetOverdrawBottomHeight(overdraw_bottom_height);
}

void RenderWidgetCompositor::SetNeedsRedrawRect(gfx::Rect damage_rect) {
  layer_tree_host_->SetNeedsRedrawRect(damage_rect);
}

void RenderWidgetCompositor::SetNeedsForcedRedraw() {
  layer_tree_host_->SetNextCommitForcesRedraw();
  setNeedsAnimate();
}

scoped_ptr<cc::SwapPromiseMonitor>
RenderWidgetCompositor::CreateLatencyInfoSwapPromiseMonitor(
    ui::LatencyInfo* latency) {
  return scoped_ptr<cc::SwapPromiseMonitor>(
      new cc::LatencyInfoSwapPromiseMonitor(
          latency, layer_tree_host_.get(), NULL));
}

int RenderWidgetCompositor::GetLayerTreeId() const {
  return layer_tree_host_->id();
}

void RenderWidgetCompositor::NotifyInputThrottledUntilCommit() {
  layer_tree_host_->NotifyInputThrottledUntilCommit();
}

const cc::Layer* RenderWidgetCompositor::GetRootLayer() const {
  return layer_tree_host_->root_layer();
}

bool RenderWidgetCompositor::ScheduleMicroBenchmark(
    const std::string& name,
    scoped_ptr<base::Value> value,
    const base::Callback<void(scoped_ptr<base::Value>)>& callback) {
  return layer_tree_host_->ScheduleMicroBenchmark(name, value.Pass(), callback);
}

bool RenderWidgetCompositor::Initialize(cc::LayerTreeSettings settings) {
  scoped_refptr<base::MessageLoopProxy> compositor_message_loop_proxy =
      RenderThreadImpl::current()->compositor_message_loop_proxy();
  if (compositor_message_loop_proxy.get()) {
    layer_tree_host_ = cc::LayerTreeHost::CreateThreaded(
        this, NULL, settings, compositor_message_loop_proxy);
  } else {
    layer_tree_host_ = cc::LayerTreeHost::CreateSingleThreaded(
        this, this, NULL, settings);
  }
  return layer_tree_host_;
}

void RenderWidgetCompositor::setSurfaceReady() {
  layer_tree_host_->SetLayerTreeHostClientReady();
}

void RenderWidgetCompositor::setRootLayer(const blink::WebLayer& layer) {
  layer_tree_host_->SetRootLayer(
      static_cast<const webkit::WebLayerImpl*>(&layer)->layer());
}

void RenderWidgetCompositor::clearRootLayer() {
  layer_tree_host_->SetRootLayer(scoped_refptr<cc::Layer>());
}

void RenderWidgetCompositor::setViewportSize(
    const WebSize&,
    const WebSize& device_viewport_size) {
  layer_tree_host_->SetViewportSize(device_viewport_size);
}

WebSize RenderWidgetCompositor::layoutViewportSize() const {
  return layer_tree_host_->device_viewport_size();
}

WebSize RenderWidgetCompositor::deviceViewportSize() const {
  return layer_tree_host_->device_viewport_size();
}

WebFloatPoint RenderWidgetCompositor::adjustEventPointForPinchZoom(
    const WebFloatPoint& point) const {
  return point;
}

void RenderWidgetCompositor::setDeviceScaleFactor(float device_scale) {
  layer_tree_host_->SetDeviceScaleFactor(device_scale);
}

float RenderWidgetCompositor::deviceScaleFactor() const {
  return layer_tree_host_->device_scale_factor();
}

void RenderWidgetCompositor::setBackgroundColor(blink::WebColor color) {
  layer_tree_host_->set_background_color(color);
}

void RenderWidgetCompositor::setHasTransparentBackground(bool transparent) {
  layer_tree_host_->set_has_transparent_background(transparent);
}

void RenderWidgetCompositor::setOverhangBitmap(const SkBitmap& bitmap) {
  layer_tree_host_->SetOverhangBitmap(bitmap);
}

void RenderWidgetCompositor::setVisible(bool visible) {
  layer_tree_host_->SetVisible(visible);
}

void RenderWidgetCompositor::setPageScaleFactorAndLimits(
    float page_scale_factor, float minimum, float maximum) {
  layer_tree_host_->SetPageScaleFactorAndLimits(
      page_scale_factor, minimum, maximum);
}

void RenderWidgetCompositor::startPageScaleAnimation(
    const blink::WebPoint& destination,
    bool use_anchor,
    float new_page_scale,
    double duration_sec) {
  base::TimeDelta duration = base::TimeDelta::FromMicroseconds(
      duration_sec * base::Time::kMicrosecondsPerSecond);
  layer_tree_host_->StartPageScaleAnimation(
      gfx::Vector2d(destination.x, destination.y),
      use_anchor,
      new_page_scale,
      duration);
}

void RenderWidgetCompositor::setNeedsAnimate() {
  layer_tree_host_->SetNeedsAnimate();
}

bool RenderWidgetCompositor::commitRequested() const {
  return layer_tree_host_->CommitRequested();
}

void RenderWidgetCompositor::didStopFlinging() {
  layer_tree_host_->DidStopFlinging();
}

void RenderWidgetCompositor::registerForAnimations(blink::WebLayer* layer) {
  cc::Layer* cc_layer = static_cast<webkit::WebLayerImpl*>(layer)->layer();
  cc_layer->layer_animation_controller()->SetAnimationRegistrar(
      layer_tree_host_->animation_registrar());
}

void RenderWidgetCompositor::registerViewportLayers(
    const blink::WebLayer* pageScaleLayer,
    const blink::WebLayer* innerViewportScrollLayer,
    const blink::WebLayer* outerViewportScrollLayer) {
  layer_tree_host_->RegisterViewportLayers(
      static_cast<const webkit::WebLayerImpl*>(pageScaleLayer)->layer(),
      static_cast<const webkit::WebLayerImpl*>(innerViewportScrollLayer)
          ->layer(),
      // The outer viewport layer will only exist when using pinch virtual
      // viewports.
      outerViewportScrollLayer ? static_cast<const webkit::WebLayerImpl*>(
                                     outerViewportScrollLayer)->layer()
                               : NULL);
}

void RenderWidgetCompositor::clearViewportLayers() {
  layer_tree_host_->RegisterViewportLayers(scoped_refptr<cc::Layer>(),
                                           scoped_refptr<cc::Layer>(),
                                           scoped_refptr<cc::Layer>());
}

bool RenderWidgetCompositor::compositeAndReadback(
    void *pixels, const WebRect& rect_in_device_viewport) {
  return layer_tree_host_->CompositeAndReadback(pixels,
                                                rect_in_device_viewport);
}

void RenderWidgetCompositor::finishAllRendering() {
  layer_tree_host_->FinishAllRendering();
}

void RenderWidgetCompositor::setDeferCommits(bool defer_commits) {
  layer_tree_host_->SetDeferCommits(defer_commits);
}

void RenderWidgetCompositor::setShowFPSCounter(bool show) {
  cc::LayerTreeDebugState debug_state = layer_tree_host_->debug_state();
  debug_state.show_fps_counter = show;
  layer_tree_host_->SetDebugState(debug_state);
}

void RenderWidgetCompositor::setShowPaintRects(bool show) {
  cc::LayerTreeDebugState debug_state = layer_tree_host_->debug_state();
  debug_state.show_paint_rects = show;
  layer_tree_host_->SetDebugState(debug_state);
}

void RenderWidgetCompositor::setShowDebugBorders(bool show) {
  cc::LayerTreeDebugState debug_state = layer_tree_host_->debug_state();
  debug_state.show_debug_borders = show;
  layer_tree_host_->SetDebugState(debug_state);
}

void RenderWidgetCompositor::setContinuousPaintingEnabled(bool enabled) {
  cc::LayerTreeDebugState debug_state = layer_tree_host_->debug_state();
  debug_state.continuous_painting = enabled;
  layer_tree_host_->SetDebugState(debug_state);
}

void RenderWidgetCompositor::setShowScrollBottleneckRects(bool show) {
  cc::LayerTreeDebugState debug_state = layer_tree_host_->debug_state();
  debug_state.show_touch_event_handler_rects = show;
  debug_state.show_wheel_event_handler_rects = show;
  debug_state.show_non_fast_scrollable_rects = show;
  layer_tree_host_->SetDebugState(debug_state);
}

void RenderWidgetCompositor::WillBeginMainFrame(int frame_id) {
  widget_->InstrumentWillBeginFrame(frame_id);
  widget_->willBeginCompositorFrame();
}

void RenderWidgetCompositor::DidBeginMainFrame() {
  widget_->InstrumentDidBeginFrame();
}

void RenderWidgetCompositor::Animate(double frame_begin_time) {
  widget_->webwidget()->animate(frame_begin_time);
}

void RenderWidgetCompositor::Layout() {
  widget_->webwidget()->layout();
}

void RenderWidgetCompositor::ApplyScrollAndScale(gfx::Vector2d scroll_delta,
                                                 float page_scale) {
  widget_->webwidget()->applyScrollAndScale(scroll_delta, page_scale);
}

scoped_ptr<cc::OutputSurface> RenderWidgetCompositor::CreateOutputSurface(
    bool fallback) {
  return widget_->CreateOutputSurface(fallback);
}

void RenderWidgetCompositor::DidInitializeOutputSurface(bool success) {
  if (!success)
    widget_->webwidget()->didExitCompositingMode();
}

void RenderWidgetCompositor::WillCommit() {
  widget_->InstrumentWillComposite();
}

void RenderWidgetCompositor::DidCommit() {
  widget_->DidCommitCompositorFrame();
  widget_->didBecomeReadyForAdditionalInput();
}

void RenderWidgetCompositor::DidCommitAndDrawFrame() {
  widget_->didCommitAndDrawCompositorFrame();
}

void RenderWidgetCompositor::DidCompleteSwapBuffers() {
  widget_->didCompleteSwapBuffers();
  if (!threaded_)
    widget_->OnSwapBuffersComplete();
}

scoped_refptr<cc::ContextProvider>
RenderWidgetCompositor::OffscreenContextProvider() {
  return RenderThreadImpl::current()->OffscreenCompositorContextProvider();
}

void RenderWidgetCompositor::ScheduleComposite() {
  if (!suppress_schedule_composite_)
    widget_->scheduleComposite();
}

void RenderWidgetCompositor::ScheduleAnimation() {
  widget_->scheduleAnimation();
}

void RenderWidgetCompositor::DidPostSwapBuffers() {
  widget_->OnSwapBuffersPosted();
}

void RenderWidgetCompositor::DidAbortSwapBuffers() {
  widget_->OnSwapBuffersAborted();
}

void RenderWidgetCompositor::RateLimitSharedMainThreadContext() {
  cc::ContextProvider* provider =
      RenderThreadImpl::current()->SharedMainThreadContextProvider().get();
  provider->Context3d()->rateLimitOffscreenContextCHROMIUM();
}

}  // namespace content
