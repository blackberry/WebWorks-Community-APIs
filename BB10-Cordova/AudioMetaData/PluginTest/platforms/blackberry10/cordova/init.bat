@ECHO OFF
goto comment
       Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.
:comment
setlocal enabledelayedexpansion

set /P CORDOVA_VERSION=<"%~dp0..\VERSION"
set "CORDOVA_HOME_DIR=!USERPROFILE!\.cordova\lib\blackberry10\cordova\!CORDOVA_VERSION!"
set "LOCAL_NODE_BINARY=!CORDOVA_HOME_DIR!\bin\dependencies\node\bin"
set "LOCAL_BBTOOLS_BINARY=!CORDOVA_HOME_DIR!\bin\dependencies\bb-tools\bin"

set FOUNDWHERE=
for /f "usebackq delims=" %%e in (`where where 2^>nul`) do (
  if not defined FOUNDWHERE set "FOUNDWHERE=%%e"
)
if not defined FOUNDWHERE set "FOUNDWHERE=%~dp0\whereis"

set FOUNDJAVAAT=
for /f "usebackq delims=" %%e in (`"%FOUNDWHERE%" java 2^>nul`) do (
  if not defined FOUNDJAVAAT set "FOUNDJAVAAT=%%~dpe"
)
:: While we could normally ask where.exe to search an arbitrary environment variable,
:: it's too complicated to teach whereis.cmd to do that
:: instead we're going to replace PATH with a special PATH
:: In case you're curious, you can't nest ::'s inside ()s
if not defined FOUNDJAVAAT (
  setlocal enabledelayedexpansion
  SET "PATH=%ProgramFiles(x86)%\java\jre7\bin;%ProgramW6432%\java\jre7\bin;"
  for /f "usebackq delims=" %%e in (`"%FOUNDWHERE%" java 2^>nul`) do (
    set "FOUNDJAVAAT=%%~dpe"
  )
  if defined FOUNDJAVAAT (
    for /f "delims=" %%C in (""!FOUNDJAVAAT!"") do (
      endlocal
      set "FOUNDJAVAAT=%%~C"
    )
    if defined FOUNDJAVAAT (
      set "PATH=!PATH!;!FOUNDJAVAAT!"
    )
  ) else (
    endlocal
  )
)
:: While there are two endlocal calls, they're mutually exclusive and bookend the setlocal

if defined CORDOVA_NODE (
  if exist "!CORDOVA_NODE!" (
    if defined CORDOVA_BBTOOLS (
      if exist "!CORDOVA_BBTOOLS!" (
        goto end
      )
    )
  )
)

if exist "!LOCAL_NODE_BINARY!" (
  set "CORDOVA_NODE=!LOCAL_NODE_BINARY!"
) else (
  for /f "usebackq delims=" %%e in (`%FOUNDWHERE% node 2^>nul`) do (
    set "CORDOVA_NODE=%%~dpe"
  )
)

if exist "!LOCAL_BBTOOLS_BINARY!" (
  set "CORDOVA_BBTOOLS=!LOCAL_BBTOOLS_BINARY!"
) else (
  for /f "usebackq delims=" %%e in (`%FOUNDWHERE% blackberry-nativepackager 2^>nul`) do (
    set "CORDOVA_BBTOOLS=%%~dpe"
  )
)

:end

if not exist "%LOCAL_NODE_BINARY%" (
  if not exist "%CORDOVA_NODE%\node.exe" (
    echo node cannot be found on the path. Aborting.
    exit /b 2
  )
)
if not defined FOUNDJAVAAT (
  echo java cannot be found on the path. Aborting.
  exit /b 2
)
if not exist "!CORDOVA_BBTOOLS!\blackberry-nativepackager.bat" (
  echo blackberry-nativepackager cannot be found on the path. Aborting.
  exit /b 2
)
if not exist "!CORDOVA_BBTOOLS!\blackberry-deploy.bat" (
  echo blackberry-deploy cannot be found on the path. Aborting.
  exit /b 2
)
if not exist "!CORDOVA_BBTOOLS!\blackberry-signer.bat" (
  echo blackberry-signer cannot be found on the path. Aborting.
  exit /b 2
)
if not exist "!CORDOVA_BBTOOLS!\blackberry-debugtokenrequest.bat" (
  echo blackberry-debugtokenrequest cannot be found on the path. Aborting.
  exit /b 2
)

:: Export variables we want to share with the caller
for /f "delims=" %%A in (""!CORDOVA_NODE!"") do (
  for /f "delims=" %%B in (""!CORDOVA_BBTOOLS!"") do (
    for /f "delims=" %%C in (""!PATH!"") do (
      endlocal
      set "CORDOVA_NODE=%%~A"
      set "CORDOVA_BBTOOLS=%%~B"
      set "PATH=%%~C"
    )
  )
)
exit /b 0
