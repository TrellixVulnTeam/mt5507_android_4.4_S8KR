/* Copyright (C) 2003 Vladimir Roubtsov. All rights reserved.
 * 
 * This program and the accompanying materials are made available under
 * the terms of the Common Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/cpl-v10.html
 * 
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/emma/core/java12/com/vladium/emma/data/ICoverageData.java#1 $
 */
package com.vladium.emma.data;

// ----------------------------------------------------------------------------
/**
 * @author Vlad Roubtsov, (C) 2003
 */
public
interface ICoverageData extends IMergeable
{
    // public: ................................................................
    
    final class DataHolder
    {
        public DataHolder (final boolean [][] coverage, final long stamp)
        {
            m_coverage = coverage;
            m_stamp = stamp;
        }
        
        public final boolean [][] m_coverage;
        public final long m_stamp;
        
    } // end of nested class
    
    Object lock ();
    
    ICoverageData shallowCopy ();
    
    int size ();
    
    /**
     * can return null
     * can return data holder with a different version stamp than cls.getStamp()
     */
    DataHolder getCoverage (ClassDescriptor cls);
    
    //void setImmutable (); // TODO: this only disables addClass(), not coverage array updates; rename
    void addClass (boolean [][] coverage, String classVMName, long stamp);

} // end of interface
// ----------------------------------------------------------------------------