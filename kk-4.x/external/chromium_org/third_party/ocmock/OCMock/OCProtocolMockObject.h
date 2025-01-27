//---------------------------------------------------------------------------------------
//  $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/chromium_org/third_party/ocmock/OCMock/OCProtocolMockObject.h#1 $
//  Copyright (c) 2005-2008 by Mulle Kybernetik. See License file for details.
//---------------------------------------------------------------------------------------

#import <OCMock/OCMockObject.h>

@interface OCProtocolMockObject : OCMockObject 
{
	Protocol	*mockedProtocol;
}

- (id)initWithProtocol:(Protocol *)aProtocol;

@end

