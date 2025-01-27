/*
 * Copyright (C) 2006 The Android Open Source Project
 * 
 * ©2010-2013 TCL CORPORATION All Rights Reserved.
 */

package android.tclwidget;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.SeekBar;
import android.view.ContextThemeWrapper;



public class TCLSeekBar extends SeekBar{

	public TCLSeekBar(Context context) {
		this(context, null);
	}

	public TCLSeekBar(Context context, AttributeSet attrs) {
		this(context, attrs, tvos.R.attr.tclseekBarStyle);
	}

	public TCLSeekBar(Context context, AttributeSet attrs, int defStyle) {
		//super(context, attrs, defStyle);
		super(new ContextThemeWrapper(context, tvos.R.style.Theme), attrs, defStyle);
	}
}
