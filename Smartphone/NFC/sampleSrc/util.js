/*
 * Copyright 2011-2012 Research In Motion Limited.
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

function setText(text) {
    var textDiv = document.getElementById("smartPosterDisplay_Text");
    clearKids(textDiv);
    textDiv.appendChild(document.createTextNode(text));
}

var uriToClick = "";

function clickURI() {
    alert("You have clicked link to " + uriToClick);
}

function setURI(uri) {
    var uriDiv = document.getElementById("smartPosterDisplay_URI");
    clearKids(uriDiv);
    var aDiv = document.createElement("a");
    aDiv.href="javascript:clickURI();";
    uriToClick = uri;
    aDiv.appendChild(document.createTextNode(uri));
    uriDiv.appendChild(aDiv);
}

function clearKids(someDiv) {
    while(someDiv.hasChildNodes()) {
        someDiv.removeChild(someDiv.firstChild);
    }
}

function addImage(uri) {
    var currentDiv = document.getElementById("smartPosterDisplay_icon");
    clearKids(currentDiv);
    if (uri!="") {
        var newImage = document.createElement("img");
        newImage.src = uri;
        currentDiv.appendChild(newImage);
    }
}

var blanketShowing = false;

function hide() {
    blanketShowing = false;
    document.getElementById("smartPosterBlanket").style.display = "none";
    document.getElementById("smartPosterDisplay").style.display = "none";
}

function show() {
    blanketShowing = true;
    var blanket = document.getElementById("smartPosterBlanket");
    blanket.style.display = "block";
    blanket.style.width = document.width;
    blanket.style.height = document.height;
    var display = document.getElementById("smartPosterDisplay"); 
    display.style.display = "block";
    display.style.left = (window.innerWidth - display.offsetWidth) / 2;
    display.style.top = (window.innerHeight - display.offsetHeight) / 2;
}

function formatJSONindent(depth) {
    var toReturn = "";
    for (var i =0; i<depth; ++i) {
        toReturn += "  ";
    }
    return toReturn;
}

function formatJSON(input) {
    var toReturn = "";
    var depth = 0;
    var colon = false;
    for (var i =0; i<input.length; ++i) {
        var c = input.charAt(i);
        if (c=='{') {
            colon = false;
            toReturn += '\n';
            toReturn += formatJSONindent(depth);
            toReturn += c;
            toReturn += '\n';
            depth += 1;
            toReturn += formatJSONindent(depth);
        } else if (c=='[') {
            colon = false;
            toReturn += c;
            toReturn += '\n';
            depth += 1;
            toReturn += formatJSONindent(depth);
        } else if (c==']') {
            toReturn += '\n';
            depth -= 1;
            toReturn += formatJSONindent(depth);
            toReturn += c;
            colon = true;
        } else if (c=='}') {
            toReturn += '\n';
            depth -= 1;
            toReturn += formatJSONindent(depth);
            toReturn += c;
            toReturn += '\n';
            colon = false;
            toReturn += formatJSONindent(depth);
        } else if (c==',' && colon) {
            toReturn += ',';
            toReturn += '\n';
            colon = false;
            toReturn += formatJSONindent(depth);
        } else {
            toReturn += c;
            if (c==':') {
                colon = true;
            }
        }
    }
    toReturn = toReturn.replace(/^ *\n/g,"");
    toReturn = toReturn.replace(/\n *\n/g,"\n");
    return toReturn;
}

