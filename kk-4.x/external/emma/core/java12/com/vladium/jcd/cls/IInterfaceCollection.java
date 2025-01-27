/* Copyright (C) 2003 Vladimir Roubtsov. All rights reserved.
 * 
 * This program and the accompanying materials are made available under
 * the terms of the Common Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/cpl-v10.html
 * 
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/emma/core/java12/com/vladium/jcd/cls/IInterfaceCollection.java#1 $
 */
package com.vladium.jcd.cls;

import com.vladium.jcd.compiler.IClassFormatOutput;

// ----------------------------------------------------------------------------
/**
 * An abstraction of the 'interfaces' component of .class format. The contents
 * are constant pool indices of {@link com.vladium.jcd.cls.constant.CONSTANT_Class_info}
 * structures corresponding to direct superinterfaces of this class/interface.
 * The order in which they appear is the left-to-right order of their declaration in
 * the implements/extends clause.
 * 
 * @author (C) 2001, Vlad Roubtsov
 */
public
interface IInterfaceCollection extends Cloneable, IClassFormatOutput
{
    // public: ................................................................

    // ACCESSORS:
    
    /**
     * Returns the {@link com.vladium.jcd.cls.constant.CONSTANT_Class_info}
     * constant pool index for offset'th direct superinterface.
     * 
     * @param offset superinterface number [must be in [0, size()) range]
     * @return constant pool index [always positive]  
     * 
     * @throws IndexOutOfBoundsException if 'offset' is outside of valid range
     */
    int get (int offset);
    
    /**
     * Returns the number of direct superinterfaces for this class/interface.
     * 
     * @return int number of direct superinterfaces [can be 0]
     */
    int size ();
    
    // Cloneable: adjust the access level of Object.clone():
    Object clone ();
    
    // Visitor:
    void accept (IClassDefVisitor visitor, Object ctx);


    // MUTATORS:
    
    /**
     * Appends a new superinterface pointer to the collection. No duplicate checks are made.
     * 
     * @param interface_index constant pool index [must be positive; input not validated]
     * @return offset of the new pointer [same as {@link #size()}-1 when called
     * after this method] 
     */
    int add (int interface_index);
    
    /**
     * Replaces superinterface pointer number 'offset' with new value 'interface_index'.
     * No duplicate checks are made. It is the responsibility of the caller to
     * ensure that the relevant CONSTANT_Class_info descriptor will be found
     * in the constant pool, in the slot pointed to by 'interface_index'.
     * 
     * @param offset offset of the superinterface pointer to replace [must be in [0, size()) range]
     * @param interface_index constant pool index [must be positive; input not validated]
     * @return previous value at the given index [always positive]
     * 
     * @throws IndexOutOfBoundsException if 'offset' is outside of valid range
     */
    int set (int offset, int interface_index);
    
} // end of interface
// ----------------------------------------------------------------------------
