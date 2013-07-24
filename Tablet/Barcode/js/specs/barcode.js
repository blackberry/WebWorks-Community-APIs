/*
* Copyright 2010-2011 Research In Motion Limited.
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

describe("webworks.media.barcode", function () {
    var barcode,
        disp,
        func = function () { };

    beforeEach(function () {
        var plugin = blackberry.Loader.getJavascript("blackberry.media.barcode");
        expect(plugin).toBeDefined();
        expect(plugin.length).toBe(2);
        disp = new plugin[1]();
        barcode = new plugin[0](disp);
    });

    describe("on the namespace object", function () {
        beforeEach(function () {
            spyOn(disp, "scan");
        });

        describe("when using the scan method", function () {
            it("throws an exception when no arguments passed in", function () {
                expect(barcode.scan).toThrow();
            });

            it("generates an id for the success callback and passes it to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(1);
                barcode.scan(func);

                expect(disp.scan).toHaveBeenCalledWith(1, undefined, undefined);
            });

            it("generates an id for the error callback and passes it to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(55);
                barcode.scan(func, func);

                expect(disp.scan).toHaveBeenCalledWith(55, 55, undefined);
            });

            it("passes the options along to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(2);
                var opts = {};
                barcode.scan(func, func, opts);

                expect(disp.scan).toHaveBeenCalledWith(2, 2, opts);
            });
        });

        describe("when using the generate method", function () {
            beforeEach(function () {
                spyOn(disp, "generate");
            });

            it("throws an exception when no arguments passed in", function () {
                expect(barcode.generate).toThrow();
            });

            it("throws an exception if no data passed in", function () {
                expect(function () {
                    barcode.generate(null, func);
                }).toThrow("must provide data");
            });

            it("throws an exception if no success callback passed in", function () {
                expect(function () {
                    barcode.generate("foo");
                }).toThrow("must provide an onCaptured callback");
            });

            it("generates an id for the capture callback and passes it to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(66);

                barcode.generate("foo", func);

                expect(disp.generate).toHaveBeenCalledWith("foo", 66, undefined, undefined);
            });

            it("generates an id for the error callback and passes it to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(66);

                barcode.generate("foo", func, func);

                expect(disp.generate).toHaveBeenCalledWith("foo", 66, 66, undefined);
            });

            it("passes the options object to the dispatcher", function () {
                spyOn(blackberry.events, "registerEventHandler").andReturn(6);
                var opts = {};
                barcode.generate("kittens", func, func, opts);

                expect(disp.generate).toHaveBeenCalledWith("kittens", 6, 6, opts);
            });
        });
    });

    describe("when using the dispatcher", function () {
        function _req() {
            return blackberry.transport.last;
        }

        beforeEach(function () {
            blackberry.transport.last = null;
        });

        describe("when calling the scan method", function () {
            it("generates the correct URI", function () {
                disp.scan(1, 0, {woo: "hoo"});
                expect(_req()._url).toBe("webworks/media/barcode/scan");
            });

            it("only adds params that have values", function () {
                disp.scan(1, 1, {});
                expect(_req().addParam.callCount).toBe(3);
                disp.scan(1, 1);
                expect(_req().addParam.callCount).toBe(2);
                disp.scan(1);
                expect(_req().addParam.callCount).toBe(1);
            });

            it("makes an asyncCall", function () {
                disp.scan(1);
                expect(_req().makeAsyncCall).toHaveBeenCalled();
            });

            it("adds the onCaptureId Param", function () {
                disp.scan(1);
                expect(_req().addParam).toHaveBeenCalledWith("onCapturedId", 1);
            });

            it("adds the onErrorId Param", function () {
                disp.scan(1, 0);
                expect(_req().addParam).toHaveBeenCalledWith("onErrorId", 0);
            });

            it("adds the options param as a string", function () {
                disp.scan(1, 2, {tryHarder: true, format: ["2d"]});
                expect(_req().addParam).toHaveBeenCalledWith("options", '{"tryHarder":true,"format":["2d"]}');
            });

            it("sets the default values", function () {
                disp.scan(1, 2, {});
                expect(_req().addParam).toHaveBeenCalledWith("options", '{"tryHarder":true,"format":["1d","2d"]}');
            });
        });

        describe("when calling the generate method", function () {
            it("generates the correct URI", function () {
                disp.generate("foo", 1);
                expect(_req()._url).toBe("webworks/media/barcode/generate");
            });

            it("only adds params that have values", function () {
                disp.generate("stuff", 1, 2, {});
                expect(_req().addParam.callCount).toBe(4);
                disp.generate("stuff", 1, 2);
                expect(_req().addParam.callCount).toBe(3);
                disp.generate("stuff", 1);
                expect(_req().addParam.callCount).toBe(2);
            });

            it("makes an asyncCall", function () {
                disp.generate("cheese", 0);
                expect(_req().makeAsyncCall).toHaveBeenCalled();
            });

            it("adds the data Param", function () {
                disp.generate("asdf", 1);
                expect(_req().addParam).toHaveBeenCalledWith("data", "asdf");
            });

            it("adds the onCapturedId Param", function () {
                disp.generate("asdf", 0);
                expect(_req().addParam).toHaveBeenCalledWith("onCapturedId", 0);
            });

            it("adds the onErrorId Param", function () {
                disp.generate("asdf", 0, 1);
                expect(_req().addParam).toHaveBeenCalledWith("onErrorId", 1);
            });

            it("adds the options param as a string", function () {
                disp.generate("data", 1, 2, {});
                expect(_req().addParam).toHaveBeenCalledWith("options", '{"width":50,"height":50}');
            });
        });
    });
});
