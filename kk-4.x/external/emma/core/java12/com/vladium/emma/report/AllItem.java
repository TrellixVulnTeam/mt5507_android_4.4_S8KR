/* Copyright (C) 2003 Vladimir Roubtsov. All rights reserved.
 * 
 * This program and the accompanying materials are made available under
 * the terms of the Common Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/cpl-v10.html
 * 
 * $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/emma/core/java12/com/vladium/emma/report/AllItem.java#1 $
 */
package com.vladium.emma.report;

// ----------------------------------------------------------------------------
/**
 * @author Vlad Roubtsov, (C) 2003
 */
public
final class AllItem extends Item
{
    // public: ................................................................
    
    public AllItem ()
    {
        super (null);
    }
    
    public String getName ()
    {
        return "all classes";
    }
        
    public void accept (final IItemVisitor visitor, final Object ctx)
    {
        visitor.visit (this, ctx);
    }
    
    // TODO: remove these instance methods returning static data
    public final IItemMetadata getMetadata ()
    {
        return METADATA;
    }

    public static IItemMetadata getTypeMetadata ()
    {
        return METADATA;
    }
        
    // protected: .............................................................

    // package: ...............................................................
    
    // private: ...............................................................
    
    
    private static final Item.ItemMetadata METADATA; // set in <clinit>
        
    static
    {        
        METADATA = new Item.ItemMetadata (IItemMetadata.TYPE_ID_ALL, "all",
            1 << IItemAttribute.ATTRIBUTE_NAME_ID |
            1 << IItemAttribute.ATTRIBUTE_CLASS_COVERAGE_ID |
            1 << IItemAttribute.ATTRIBUTE_METHOD_COVERAGE_ID |
            1 << IItemAttribute.ATTRIBUTE_BLOCK_COVERAGE_ID |
            1 << IItemAttribute.ATTRIBUTE_LINE_COVERAGE_ID);
    }

} // end of class
// ----------------------------------------------------------------------------