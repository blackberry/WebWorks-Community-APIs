/*
 * Copyright 2013 Research In Motion Limited.
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
#ifndef AES_HPP_
#define AES_HPP_

#include "provider.hpp"

namespace gsecrypto {

class AES : public Provider {
public:
	AES(class GSECrypto & owner);
	virtual ~AES();

	virtual bool doesSupport(const std::string &);

	virtual Json::Value generateKey(const std::string & alg, Json::Value & args);
	virtual Json::Value encrypt(const std::string & alg, Json::Value & args);
	virtual Json::Value decrypt(const std::string & alg, Json::Value & args);

	friend class AESParams;
	friend class AESKey;
	friend class AESContext;

private:
	Json::Value crypt(const std::string & alg, Json::Value & args, bool isEncrypt);
};

class AESParams {
public:
	AESParams(AES & owner);
	virtual ~AESParams();

	void create(int mode, size_t blockLen, bool withRandom);

private:
	AES & owner;
	sb_Params params;

	friend class AESKey;
	friend class AESContext;
};

class AESKey {
public:
	AESKey(AESParams & owner, size_t size);
	AESKey(AESParams & owner, DataTracker & dt);
	virtual ~AESKey();
	void get(DataTracker & dt);
private:
	AESParams & params;
	sb_Key key;

	friend class AESContext;
};

class AESContext {
public:
	AESContext(AESParams &, AESKey &, int mode, DataTracker & iv);
	virtual ~AESContext();

	void crypt(DataTracker & in, DataTracker & out, bool isEncrypt);

private:
	AESParams & params;
	sb_Context context;
};

} /* namespace gsecrypto */
#endif /* AES_HPP_ */
