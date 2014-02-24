# GSE Crypto

This extension allows WebWorks code to call features of the cryptographic library included in BlackBerry 10.

Currently implemented algorithms include:

* Crytographic Hash Functions
    * MD5 (**not** recommended for cryptographic use)
    * SHA1 (**not** recommended for cryptographic use)
    * SHA2 (224/256/384/512 bits)
* Block Ciphers
    * AES-CBC 128/192/256 (coming soon)

# Getting Started
## Warning
Use this cryptographic library at your own risk. While native functions are FIPS validated, it's possible that this extension has _somehow_ introduced errors, unlikely as that may seem given the testing done. In addition, if you plan to use any of these functions, make sure that you know what you are doing: poorly used the security of these algorithms mean nothing.

## Data

Cryptographic data is binary. You need to be clear on which bytes you want to work with. For this reason the extension accepts data as hexadecimal (hex) or Base64 (b64) encoding of the bytes.

Hexadecimal is easier to work with since concatenating two values is as simple as concatenating the strings. Eg:

```javascript
{
    "input" : {
        "hex" : "b0b5cafeb0b5cafeb0b5cafe"
    }
}
```

Base64 is the preferred encoding of MIME. Some features of the Invocation Framework expect b64 encoded data. Data URIs have their b64 encoded. Eg:

```javascript
{
    "input" : {
        "b64" : "sLXK/rC1yv6wtcr+"
    }
}
```

Binary data coming back from the extension is available as in both encodings: hex and b64.

## Errors

Errors will be given as a member of the result object. Eg:

```javascript
{
    "error" : "This is the error message"
}
```

# Operations
Current operations you can call are:

* hash
* (others coming soon)

The native API can be found [here](http://developer.blackberry.com/native/reference/core/com.qnx.doc.crypto.lib_ref/topic/manual/intro.html). The library contains a plethora of functions, and if there is something not yet implemented here that you would like to see, please feel free to make a request.

## hash

hash expects an object with algorithm (alg) and input data (input) set.

Algorithm options are: "md5", "sha1", "sha224", "sha256", "sha384" or "sha512"

Input can be hex, b64 (or both but only the hex will be used). Eg:

```javascript
{
    "alg" : "sha256",
    "input" : {
        "hex" : "0123abcd"
    }
};
```

The (non-error) output is an object with an attribute called output. Output itself is an object containing a hex and b64 encoding of the output. Eg:
```javascript
{
    "output" : {
        "b64":"ZOqwcFOUUBztD/mRv2kHf9OEbB2WTj2yjZYAiRcV2Eg=",
        "hex":"64eab0705394501ced0ff991bf69077fd3846c1d964e3db28d9600891715d848"
    }
}
```

# Usage Examples
See gseCrypto/sample/www/js/index.js for examples of all operations in use.

