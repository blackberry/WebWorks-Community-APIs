# GSE Crypto

This extension allows WebWorks code to call features of the cryptographic library included in BlackBerry 10.

Currently implemented algorithms include:
* All SHA
 * SHA1, SHA224, SHA256, SHA384, SHA512
* AES CBC
 * All key sizes. (128, 192, 256 bit)

# Getting Started
## Data

Cryptographic data is binary. You need to be clear on which bytes you want to work with. For this reason the extension accepts data as any of hex, b64 or raw Strings. 

Hex is a hexadecimal encoding of the bytes. It’s the safest to work with, and in some ways the easiest. Concatenating two values in hex is way easier than in b64…

```javascript
{ input: { hex: "b0b5cafe" } }
```

Base 64 [b64] is the preferred encoding of MIME. Some features of the Invocation Framework expect b64 encoded data. Data URIs have their b64 encoded. If a crypto extension can work with b64 – that’s probably going to come in handy.

```javascript
{input: { b64: "aAz9" } }
```

Raw – is just passing the library your String. The big catch here is UTF-8 encoding happening out of your control. If some cipher text contains non-valid UTF-8 encodings – how are you going to marshal that string between JavaScript and c++? You can use it – but trying to use raw with binary data – or non low ASCII languages may bite you later!

```javascript
{ input: { raw: "Montréal" } }
```

Binary data coming back from the extension is available as all 3 encodings, hex, b64 and raw. (Still not sure that raw is a good idea :P )

## Errors

Errors will be given as a member of the result object. You’ll see this in the sample application;

```javascript
if ("error" in result) {
    alert(result.error);
    return;
}
```

# Operations
Current operations you can call are;
* hash
*	generateKey
*	encrypt
*	decrypt
*	random

## hash

Hash expects an object with algorithm (alg) and input data (input) set. Eg:
```javascript
{
    alg : "sha256",
    input : {
        raw : "r"
    }
};
```

The typical result is an object with an attribute called output. Output itself is an object containing a hex, b64 and raw encoding of the output. Eg:
```javascript
{
    "output" : {
        "b64":"RUNJ5CLwUpcZHq0T4h09tSDlq+9SBV5JZLgvshP1k6E=",
        "hex":"454349e422f05297191ead13e21d3db520e5abef52055e4964b82fb213f593a1",
        "raw":"ECIä\"ðR\u0019\u001e¬\u0013â\u001d=µ å«ïR\u0005^Id¸/²\u0013õ¡"
    }
}
```

## generateKey

Generate key expects an object with algorithm (alg) set. Each individual key generator may expect more values to be set.

### generateKey – AES
Generate key – AES – expects the algorithm (alg) and key size (keySize) in bits as an integer to be set. Eg:
```javascript
{
    alg : "aes",
    keySize : 192
}
```

AES keys are symmetrical. The data comprising the key is contained in the key attribute of the returned object.
```javascript
{
    "key" : {
        "b64" : "yzC1yYivNK1VV8gNlYYaSQ==",
        "hex" : "cb30b5c988af34ad5557c80d95861a49",
        "raw" : "Ë0µÉ¯4¬UWÈ\r\u001aI"
    }
}
```

Generating an AES key is redundant – you could have just used random data of the correct length.

## encrypt
Encrypt expects an object with algorithm (alg) set. Each individual encryption implementation may expect more values to be set.

### encrypt – AES
Encrypt – AES expects the following values in the parameter object.
* Algorithm (alg) 
* Mode (mode) (only CBC is currently implemented)
* Key (key)
* Initialization Vector (iv) (only CBC is current implemented – so this is required)
* Data to encrypt (input); Eg:

```javascript
{
    "alg" : "aes",
    "mode" : "cbc",
    "key" : {
        "hex":"cb30b5c988af34ad5557c80d95861a49"
    },
    "input" : {
        "b64":"TW9udHLDqWFsBwcHBwcHBw=="
    },
    "iv" : {
        "hex":"ffeeddccbbaa99887766554433221100"
    }
}
```

The typical results object contains data in the output attribute. Eg:

```javascript
{
    "output" : {
        "b64" : "U1fXXVAxdjJf3CLLnF/SVw==",
        "hex" : "5357d75d503176325fdc22cb9c5fd257",
        "raw" : "SW×]P1v2_Ü\"Ë_ÒW"
    }
}
```

## decrypt

Decrypt expects an object with algorithm (alg) set. Each individual decryption implementation may expect more values to be set.

### decrypt – AES

Decrypt – AES expects and returns the same attributes as encrypt – AES.

## random

Random does not expect an algorithm (alg) to be set on input. The original extension author was too lazy to allow the random implementation to be selected.

Random does expect the attribute size to be set. It is an integer indicating the number of random byte that should be returned.

```javascript
{
    size : 16
}
```

A response will have the output attribute set to the random data. Eg:
```javascript
{
    "output" : {
        "b64" : "QGXBbFgY3FPkNwuZHtGRYA==",
        "hex" : "4065c16c5818dc53e4370b991ed19160",
        "raw" : "@eÁlX\u0018ÜSä7\u000b\u001eÑ`"
    }
}
```

# Padding
Security Builder GSE does not implement padding algorithms for block ciphers. For this reason you’ll see a PKCS#5 padding implementation in the JavaScript of the sample application.

This padding implementation shows a safe workflow – the function UTF-8 encodes a string before it is padded. It then encodes the combined block using b64.

The padding verification does the reverse – refusing to return the alleged plain text unless the padding verifies.

Remember that padding isn’t really much security… If you try to verify the padding on random bytes the probability that it will pass is great than 1/256…



[![Analytics](https://ga-beacon.appspot.com/UA-46817652-1/WebWorks-Community-APIs/BB10/GSECrypto?pixel)](https://github.com/igrigorik/ga-beacon)