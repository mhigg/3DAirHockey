/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2019 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

namespace ExitGames
{
	namespace Common
	{
		class JString;
	}

	namespace LoadBalancing
	{
		class AuthenticationValues;
		class Client;

		namespace Internal
		{
			class AuthenticationValuesSecretSetter
			{
				static void setSecret(AuthenticationValues& authenticationValues, const Common::JString& secret);

				friend class LoadBalancing::Client;
			};
		}
	}
}