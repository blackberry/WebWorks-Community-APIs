/*
 * Copyright 2013-2014 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef DES_HPP_
#define DES_HPP_

#include "provider.hpp"

namespace gsecrypto {

/**
 * DES provider to plug into GSECryptoNDK
 */
class DES: public Provider {
public:
	DES(class GSECryptoNDK & owner);
	virtual ~DES();

	virtual bool doesSupport(const std::string &);
	virtual Json::Value encrypt(const std::string & alg, Json::Value & args);
	virtual Json::Value decrypt(const std::string & alg, Json::Value & args);

	friend class DESParams;
	friend class DESKey;
	friend class DESContext;

private:
	Json::Value crypt(const std::string & alg, Json::Value & args,
			bool isEncrypt);
};

/**
 * c++ wrapper for DES sb_Params
 */
class DESParams {
public:
	DESParams(DES & owner, int alg, int mode, size_t blockLen, bool withRandom);
	virtual ~DESParams();

private:
	DES & owner;
	sb_Params params;

	friend class DESKey;
	friend class DESContext;
};

/**
 * c++ wrapper for DES sb_key
 */
class DESKey {
public:
	DESKey(DESParams & owner, DataTracker & dt1, DataTracker & dt2, DataTracker & dt3);
	virtual ~DESKey();

private:
	DESParams & params;
	sb_Key key;

	friend class DESContext;
};

/**
 * c++ wrapper for DES sb_Context
 */
class DESContext {
public:
	DESContext(DESParams &, DESKey &, int mode, DataTracker & iv);
	virtual ~DESContext();

	void crypt(DataTracker & in, DataTracker & out, bool isEncrypt);

private:
	DESParams & params;
	sb_Context context;
};

} /* namespace gsecrypto */
#endif /* DES_HPP_ */

