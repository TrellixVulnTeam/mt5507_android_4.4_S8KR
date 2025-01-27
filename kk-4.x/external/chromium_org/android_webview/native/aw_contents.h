// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_NATIVE_AW_CONTENTS_H_
#define ANDROID_WEBVIEW_NATIVE_AW_CONTENTS_H_

#include <jni.h>
#include <list>
#include <string>
#include <utility>

#include "android_webview/browser/browser_view_renderer.h"
#include "android_webview/browser/find_helper.h"
#include "android_webview/browser/icon_helper.h"
#include "android_webview/browser/renderer_host/aw_render_view_host_ext.h"
#include "base/android/scoped_java_ref.h"
#include "base/android/jni_helper.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

class SkBitmap;
class TabContents;

namespace content {
class WebContents;
}

namespace android_webview {

class AwContentsContainer;
class AwContentsClientBridge;
class AwPdfExporter;
class AwWebContentsDelegate;

// Native side of java-class of same name.
// Provides the ownership of and access to browser components required for
// WebView functionality; analogous to chrome's TabContents, but with a
// level of indirection provided by the AwContentsContainer abstraction.
//
// Object lifetime:
// For most purposes the java and native objects can be considered to have
// 1:1 lifetime and relationship. The exception is the java instance that
// hosts a popup will be rebound to a second native instance (carrying the
// popup content) and discard the 'default' native instance it made on
// construction. A native instance is only bound to at most one Java peer over
// its entire lifetime - see Init() and SetPendingWebContentsForPopup() for the
// construction points, and SetJavaPeers() where these paths join.
class AwContents : public FindHelper::Listener,
                   public IconHelper::Listener,
                   public AwRenderViewHostExtClient,
                   public BrowserViewRenderer::Client {
 public:

  class Receiver {
   public:
    virtual void SetSaveDir(const std::string& path) = 0;
  };

  // Returns the AwContents instance associated with |web_contents|, or NULL.
  static AwContents* FromWebContents(content::WebContents* web_contents);

  // Returns the AwContents instance associated with with the given
  // render_process_id and render_view_id, or NULL.
  static AwContents* FromID(int render_process_id, int render_view_id);

  AwContents(scoped_ptr<content::WebContents> web_contents);
  virtual ~AwContents();

  AwRenderViewHostExt* render_view_host_ext() {
    return render_view_host_ext_.get();
  }

  // |handler| is an instance of
  // org.chromium.android_webview.AwHttpAuthHandler.
  bool OnReceivedHttpAuthRequest(const base::android::JavaRef<jobject>& handler,
                                 const std::string& host,
                                 const std::string& realm);

  // Methods called from Java.
  void SetJavaPeers(JNIEnv* env,
                    jobject obj,
                    jobject aw_contents,
                    jobject web_contents_delegate,
                    jobject contents_client_bridge,
                    jobject io_thread_client,
                    jobject intercept_navigation_delegate);
  jint GetWebContents(JNIEnv* env, jobject obj);

  void Destroy(JNIEnv* env, jobject obj);
  void DocumentHasImages(JNIEnv* env, jobject obj, jobject message);
  void GenerateMHTML(JNIEnv* env, jobject obj, jstring jpath, jobject callback);
  /// M: add save page interface @{
  bool SavePage(JNIEnv* env, jobject obj);

  // callback from download manager delegate
  void GetSaveDir(bool can_save_as_complete);

  void SetSaveDir(JNIEnv* env, jobject obj, jstring value);

  void SetReceiver(Receiver* r);

  void OnSavePageStart(const uint32 id, const std::string& path);

  void OnSaveProgressChange(const uint32 id, const int progress);

  void OnSaveFinish(const uint32 id, const int flag);

  bool IsDestroy() {
    return is_destroy_;
  }
  /// @}
  void CreatePdfExporter(JNIEnv* env, jobject obj, jobject pdfExporter);
  void AddVisitedLinks(JNIEnv* env, jobject obj, jobjectArray jvisited_links);
  base::android::ScopedJavaLocalRef<jbyteArray> GetCertificate(
      JNIEnv* env, jobject obj);
  void RequestNewHitTestDataAt(JNIEnv* env, jobject obj, jint x, jint y);
  void UpdateLastHitTestData(JNIEnv* env, jobject obj);
  void OnSizeChanged(JNIEnv* env, jobject obj, int w, int h, int ow, int oh);
  void SetViewVisibility(JNIEnv* env, jobject obj, bool visible);
  void SetWindowVisibility(JNIEnv* env, jobject obj, bool visible);
  void SetIsPaused(JNIEnv* env, jobject obj, bool paused);
  void OnAttachedToWindow(JNIEnv* env, jobject obj, int w, int h);
  void OnDetachedFromWindow(JNIEnv* env, jobject obj);
  base::android::ScopedJavaLocalRef<jbyteArray> GetOpaqueState(
      JNIEnv* env, jobject obj);
  jboolean RestoreFromOpaqueState(JNIEnv* env, jobject obj, jbyteArray state);
  void FocusFirstNode(JNIEnv* env, jobject obj);
  void SetBackgroundColor(JNIEnv* env, jobject obj, jint color);
  bool OnDraw(JNIEnv* env,
              jobject obj,
              jobject canvas,
              jboolean is_hardware_accelerated,
              jint scroll_x,
              jint scroll_y,
              jint clip_left,
              jint clip_top,
              jint clip_right,
              jint clip_bottom);
  void SetGlobalVisibleRect(JNIEnv* env,
                            jobject obj,
                            jint visible_left,
                            jint visible_top,
                            jint visible_right,
                            jint visible_bottom);
  jint GetAwDrawGLViewContext(JNIEnv* env, jobject obj);
  jlong CapturePicture(JNIEnv* env, jobject obj, int width, int height);
  void EnableOnNewPicture(JNIEnv* env, jobject obj, jboolean enabled);
  void SetExtraHeadersForUrl(JNIEnv* env, jobject obj,
                             jstring url, jstring extra_headers);

  // Geolocation API support
  void ShowGeolocationPrompt(const GURL& origin, base::Callback<void(bool)>);
  void HideGeolocationPrompt(const GURL& origin);
  void InvokeGeolocationCallback(JNIEnv* env,
                                 jobject obj,
                                 jboolean value,
                                 jstring origin);

  // Find-in-page API and related methods.
  void FindAllAsync(JNIEnv* env, jobject obj, jstring search_string);
  void FindNext(JNIEnv* env, jobject obj, jboolean forward);
  void ClearMatches(JNIEnv* env, jobject obj);
  FindHelper* GetFindHelper();

  // FindHelper::Listener implementation.
  virtual void OnFindResultReceived(int active_ordinal,
                                    int match_count,
                                    bool finished) OVERRIDE;
  // IconHelper::Listener implementation.
  virtual bool ShouldDownloadFavicon(const GURL& icon_url) OVERRIDE;
  virtual void OnReceivedIcon(const GURL& icon_url,
                              const SkBitmap& bitmap) OVERRIDE;
  virtual void OnReceivedTouchIconUrl(const std::string& url,
                                      const bool precomposed) OVERRIDE;

  // AwRenderViewHostExtClient implementation.
  virtual void OnWebLayoutPageScaleFactorChanged(
      float page_scale_factor) OVERRIDE;
  virtual void OnWebLayoutContentsSizeChanged(
      const gfx::Size& contents_size) OVERRIDE;

  // BrowserViewRenderer::Client implementation.
  virtual bool RequestDrawGL(jobject canvas) OVERRIDE;
  virtual void PostInvalidate() OVERRIDE;
  virtual void UpdateGlobalVisibleRect() OVERRIDE;
  virtual void OnNewPicture() OVERRIDE;
  virtual gfx::Point GetLocationOnScreen() OVERRIDE;
  virtual void SetMaxContainerViewScrollOffset(
      gfx::Vector2d new_value) OVERRIDE;
  virtual void ScrollContainerViewTo(gfx::Vector2d new_value) OVERRIDE;
  virtual bool IsFlingActive() const OVERRIDE;
  virtual void SetPageScaleFactor(float page_scale_factor) OVERRIDE;
  virtual void SetContentsSize(gfx::SizeF contents_size_dip) OVERRIDE;
  virtual void DidOverscroll(gfx::Vector2d overscroll_delta) OVERRIDE;

  void ClearCache(JNIEnv* env, jobject obj, jboolean include_disk_files);
  void SetPendingWebContentsForPopup(scoped_ptr<content::WebContents> pending);
  jint ReleasePopupAwContents(JNIEnv* env, jobject obj);

  void ScrollTo(JNIEnv* env, jobject obj, jint x, jint y);
  void SetDipScale(JNIEnv* env, jobject obj, jfloat dip_scale);
  void SetFixedLayoutSize(JNIEnv* env,
                          jobject obj,
                          jint width_dip,
                          jint height_dip);
  void SetSaveFormData(bool enabled);

  // Sets the java delegate
  void SetAwAutofillManagerDelegate(jobject delegate);

  void SetJsOnlineProperty(JNIEnv* env, jobject obj, jboolean network_up);
  void TrimMemory(JNIEnv* env, jobject obj, jint level);

  /// M: dump memory status
  void DumpMemoryStatus(JNIEnv* env, jobject obj);

 private:
  void InitAutofillIfNecessary(bool enabled);
  void SetAndroidWebViewRendererPrefs();

  JavaObjectWeakGlobalRef java_ref_;
  scoped_ptr<content::WebContents> web_contents_;
  scoped_ptr<AwWebContentsDelegate> web_contents_delegate_;
  scoped_ptr<AwContentsClientBridge> contents_client_bridge_;
  scoped_ptr<AwRenderViewHostExt> render_view_host_ext_;
  scoped_ptr<FindHelper> find_helper_;
  scoped_ptr<IconHelper> icon_helper_;
  scoped_ptr<AwContents> pending_contents_;
  scoped_ptr<BrowserViewRenderer> browser_view_renderer_;
  scoped_ptr<AwPdfExporter> pdf_exporter_;

  // GURL is supplied by the content layer as requesting frame.
  // Callback is supplied by the content layer, and is invoked with the result
  // from the permission prompt.
  typedef std::pair<const GURL, base::Callback<void(bool)> > OriginCallback;
  // The first element in the list is always the currently pending request.
  std::list<OriginCallback> pending_geolocation_prompts_;

  /// M: add save page client
  Receiver* receiver_;

  bool is_destroy_;

  DISALLOW_COPY_AND_ASSIGN(AwContents);
};

bool RegisterAwContents(JNIEnv* env);

}  // namespace android_webview

#endif  // ANDROID_WEBVIEW_NATIVE_AW_CONTENTS_H_
