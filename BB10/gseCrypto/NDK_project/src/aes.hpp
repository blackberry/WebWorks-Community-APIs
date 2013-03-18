/*
 * AES.hpp
 *
 *  Created on: 2013-03-13
 *      Author: robwilliams
 */

#ifndef AES_HPP_
#define AES_HPP_

#include "provider.hpp"

namespace webworks {

class AES : public Provider {
public:
	AES(class GSECrypto & owner);
	virtual ~AES();

	virtual bool doesSupport(const std::string &);

	virtual Json::Value generateKey(const std::string & alg, Json::Value & args);
	virtual Json::Value encrypt(const std::string & alg, Json::Value & args);

	friend class AESParams;
	friend class AESKey;
	friend class AESContext;
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

	void encrypt(DataTracker & in, DataTracker & out);

private:
	AESParams & params;
	sb_Context context;
};

} /* namespace webworks */
#endif /* AES_HPP_ */
