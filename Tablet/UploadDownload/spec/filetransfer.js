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

describe("blackberry.io.fileTransfer", function () {
    var ft,
        disp,
        _func = function () {};

    beforeEach(function() {
        var plugins = blackberry.Loader.getJavascript("blackberry.io.fileTransfer");

        expect(plugins).toBeDefined();
        expect(plugins.length).toBe(2);
            disp = new plugins[1]();
            ft = new plugins[0](disp);
    });
        
    describe("on the namespace object", function () {
        var _args,
            _method;
        
        beforeEach(function () {
            _args = undefined;
            method = undefined;
        });


        function _paramTester() {
            ft[_method].apply(null, _args);
        }

        describe("when using the download method", function () {
            beforeEach(function () {
                _method = "download";
            });

            it("throws an exception if remotePath is not provided", function () {
                _args = [null, "foo", _func];
                expect(_paramTester).toThrow("remotePath is needed");
            });

            it("throws an exception if localPath is not provided", function () {
                _args = ["foo", null, _func];
                expect(_paramTester).toThrow("localPath is needed");
            });

            it("throws an exception if onProgress is not provided", function () {
                _args = ["foo", "bar", null];
                expect(_paramTester).toThrow("onProgress is needed");
            });

            it("throws an exception if onProgress is not a function", function () {
                _args = ["foo", "bar", "moo"];
                expect(_paramTester).toThrow("onProgress must be a function");
            });

            it("calls the download method on the dispatcher", function () {
                spyOn(disp, "download").andReturn(1);

                ft.download("foo", "bar", _func, _func, {});
                expect(disp.download.callCount).toBe(1);
            });

            it("passes the parameters correctly to the dispatcher", function () {
                var options = {option1: "ftw"};

                spyOn(disp, "download").andReturn(1);

                ft.download("foo", "bar", _func, _func, options);
                expect(disp.download).toHaveBeenCalledWith("foo", "bar", _func, _func, options);
            });
        });

        describe("when using the upload method", function () {
            beforeEach(function () {
                _method = "upload";
            });

            it("throws an exception if remotePath is not provided", function () {
                _args = [null, "foo", _func];
                expect(_paramTester).toThrow("remotePath is needed");
            });

            it("throws an exception if localPath is not provided", function () {
                _args = ["foo", null, _func];
                expect(_paramTester).toThrow("localPath is needed");
            });

            it("throws an exception if onProgress is not provided", function () {
                _args = ["foo", "bar", null];
                expect(_paramTester).toThrow("onProgress is needed");
            });

            it("throws an exception if onProgress is not a function", function () {
                _args = ["foo", "bar", "moo"];
                expect(_paramTester).toThrow("onProgress must be a function");
            });

            it("calls the download method on the dispatcher", function () {
                spyOn(disp, "download").andReturn(1);

                ft.download("foo", "bar", _func, _func, {});
                expect(disp.download.callCount).toBe(1);
            });

            it("passes the parameters correctly to the dispatcher", function () {
                var options = {option1: "ftw"};

                spyOn(disp, "download").andReturn(1);

                ft.download("foo", "bar", _func, _func, options);
                expect(disp.download).toHaveBeenCalledWith("foo", "bar", _func, _func, options);
            });
        });
    });

    describe("on the dispatcher object", function () {
        function _req() {
            return blackberry.transport.last;
        }

        beforeEach(function () {
            blackberry.transport.last = null;
            spyOn(blackberry.events, "registerEventHandler").andReturn(66);
        });
        
        describe("when using the download method", function () {
            it("generates an id for the onProgress callback", function () {
                disp.download("foo", "bar", _func);
                expect(blackberry.events.registerEventHandler.callCount).toBe(1);
            });

            it("generates an id for both callbacks", function () {
                disp.download("foo", "bar", _func, _func);
                expect(blackberry.events.registerEventHandler.callCount).toBe(2);
            });

            it("only adds the correct number of parameters", function () {
                disp.download("foo", "bar", _func, _func, {});
                expect(_req().addParam.callCount).toBe(5);
                disp.download("foo", "bar", _func, _func);
                expect(_req().addParam.callCount).toBe(4);
                disp.download("foo", "bar", _func);
                expect(_req().addParam.callCount).toBe(3);

            });

            it("adds the remote path Param", function () {
                disp.download("foo", "bar", _func);
                expect(_req().addParam).toHaveBeenCalledWith("REMOTE_PATH", "foo");
            });

            it("adds the local path Param", function () {
                disp.download("foo", "bar", _func);
                expect(_req().addParam).toHaveBeenCalledWith("LOCAL_PATH", "bar");
            });

            it("adds the on progress id Param", function () {
                disp.download("foo", "bar", _func);
                expect(_req().addParam).toHaveBeenCalledWith("ON_PROGRESS_ID", 66);
            });

            it("adds the on error id Param", function () {
                disp.download("foo", "bar", _func, _func);
                expect(_req().addParam).toHaveBeenCalledWith("ON_ERROR_ID", 66);
            });

            it("adds the options Param", function () {
                disp.download("foo", "bar", _func, _func, {});
                expect(_req().addParam).toHaveBeenCalledWith("OPTIONS", JSON.stringify({}));
            });
        })
    });
});
