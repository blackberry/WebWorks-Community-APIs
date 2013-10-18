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

#include "pgaudio_js.hpp"

#include <QDir>
#include <qdebug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sstream>

using namespace std;

/**
 * Default constructor.
 */
PGaudio::PGaudio(const std::string& id) : m_id(id)
{
    // Initalize the ALUT and creates OpenAL context on default device
    // Input 0,0 so it grabs the native device as default and creates the context automatically
    alutInit(0, 0);
}

/**
 * Destructor.
 */
PGaudio::~PGaudio()
{
    ALuint bufferID = 0;
    ALuint sources = 0;

    // Clear the buffers and sources while iterating through the hash table.
    QHashIterator<QString, ALuint> bufferIterator(m_soundBuffersHash);

    while (bufferIterator.hasNext()) {
        bufferIterator.next();

        // Get the buffer id and delete it.
        bufferID = m_soundBuffersHash[bufferIterator.key()];
        if (bufferID)
            alDeleteBuffers(1, &bufferID);
    }

    QHashIterator<QString, ALuint> sourceIterator(m_sourceIndexHash);
    while (sourceIterator.hasNext()) {
        sourceIterator.next();

        // Get the buffer id and delete it.
        sources = m_sourceIndexHash[sourceIterator.key()];
        if (sources)
            alDeleteSources(1, &sources);
    }

    // Clear the QHash for sound buffer IDs.
    m_soundBuffersHash.clear();
    m_sourceIndexHash.clear();

    // Exit the ALUT.
    alutExit();
}

/**
 * This method returns the list of objects implemented by this native
 * extension.
 */
char* onGetObjList()
{
    static char name[] = "PGaudio";
    return name;
}

/**
 * This method is used by JNext to instantiate the object when
 * an object is created on the JavaScript server side.
 */
JSExt* onCreateObject(const string& className, const string& id)
{
    if (className == "PGaudio")
        return new PGaudio(id);
    return 0;
}

/**
 * Method used by JNext to determine if the object can be deleted.
 */
bool PGaudio::CanDelete()
{
    return true;
}

// Preload function. Takes in three parameters, the path to sound file, sound file name, how many voices it contains.
string PGaudio::preload(QString path, QString fileName, int voices)
{
    QString applicationDirectory;
    char cwd[PATH_MAX];
    ALuint bufferID;
    ALuint source;

    // Check to see if the file has already been preloaded
    // Append the applications directory to the www's sound files directory.
    if (m_soundBuffersHash[fileName])
        return "Already preloaded " + fileName.toStdString();

    // First, we get the complete application directory in which we will load sounds from then
    // we convert to QString since it is more convenient when working with directories.
    // Append the applications directory to the www's sound files directory.
    getcwd(cwd, PATH_MAX);
    applicationDirectory = QString(cwd).append("/app/native/").append(path);

    // Create OpenAL buffers from all files in the sound directory.
    QDir dir(applicationDirectory);

    // Incorrect path check
    if (!dir.exists())
        return "Not loaded. Could not find path " + path.toStdString();

    // Set a filter for file listing, only files should be listed.
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    // Create the Unique Buffer ID from the path
    bufferID = alutCreateBufferFromFile(applicationDirectory.append(fileName).toStdString().c_str());

    // Create sources and buffers corresponding to each unique file name.
    m_soundBuffersHash[fileName] = bufferID;
    for (int i = 0; i < voices; i++) {
        // Generate the sources, one by one, and store them into m_sourceIndexHash, our hash, using insert Multi hashing
        // Also assign the bufferID to the sources
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, bufferID);
        m_sourceIndexHash.insertMulti(fileName, source);
    }

    return "loaded " + fileName.toStdString();
}

string PGaudio::unload(QString fileName)
{
    // Get corresponding buffers, voices and sources from the unique file name.
    ALuint bufferID = m_soundBuffersHash[fileName];

    // Loop to make sure every source is deleted incase it had multiple voices.
    QList<ALuint> sources = m_sourceIndexHash.values(fileName);
    for (int i = 0; i < sources.size(); ++i)
        alDeleteSources(1, &sources.at(i));

    // Delete sources and buffers.
    alDeleteBuffers(1, &bufferID);
    m_sourceIndexHash.remove(fileName);

    // Re-initalize the buffers.
    m_soundBuffersHash[fileName] = 0;
    return "Unloading " + fileName.toStdString();
}

// Function to stop playing sounds. Takes in sound file name.
string PGaudio::stop(QString fileName)
{
    // Loop and stop every sound with corresponding fileName.
    QList<ALuint> sources = m_sourceIndexHash.values(fileName);
    for (int i = 0; i < sources.size(); ++i)
        alSourceStop(sources.at(i));

    // Stopped playing source.
    return "Stopped " + fileName.toStdString();
}

// Function to get Duration. Takes in sound file name.
float PGaudio::getDuration(QString fileName)
{

    ALuint bufferID = m_soundBuffersHash[fileName];
    QList<ALuint> sources = m_sourceIndexHash.values(fileName);
    ALuint source = sources.at(sources.size() - 1);

    ALint bufferSize, frequency, bitsPerSample, channels;
    alGetBufferi(bufferID, AL_SIZE, &bufferSize);
    alGetBufferi(bufferID, AL_FREQUENCY, &frequency);
    alGetBufferi(bufferID, AL_CHANNELS, &channels);    
    alGetBufferi(bufferID, AL_BITS, &bitsPerSample);   

    float result = ((double)bufferSize) / (frequency * channels * (bitsPerSample / 8));
    return result;
}

// Function to play single sound. Takes in one parameter, the sound file name.
string PGaudio::play(QString fileName)
{
    float currentTime = 0, furthestTime = 0;
    ALuint replayingSource = 0;

    // Get corresponding buffer from the unique file name.
    ALuint bufferID = m_soundBuffersHash[fileName];

    // Check to see if it has been preloaded.
    if (!bufferID)
        return "Could not find the file " + fileName.toStdString() + " . Maybe it hasn't been loaded.";

    // Iterate through a list of sources associated with with the file and play the sound if it is not currently playing
    QList<ALuint> sources = m_sourceIndexHash.values(fileName);
    for (int i = 0; i < sources.size(); ++i) {
        ALenum state;
        alGetSourcef(sources.at(i), AL_SEC_OFFSET, &currentTime);
        alGetSourcei(sources.at(i), AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            alSourcePlay(sources.at(i));
            return "Playing " + fileName.toStdString();
        }
        if (currentTime > furthestTime) {
            furthestTime = currentTime;
            replayingSource = sources.at(i);
        }
    }

    // Continue cycling through and overwrite the sources if all sources are currently being used
    alSourcePlay(replayingSource);
    return "Every single voice is currenty being played, now overwriting previous ones";
}

// Function to loop sound. Takes in one parameter, the sound file name.
string PGaudio::loop(QString fileName)
{
    // Get corresponding buffers and sources from the unique file name.
    ALuint bufferID = m_soundBuffersHash[fileName];

    // If sound file has been preloaded loop sound
    if (!bufferID)
        return "Could not find the file " + fileName.toStdString() + " . Maybe it hasn't been loaded.";
    
    // Get the source from which the sound will be played.
    ALuint source;
    QList<ALuint> sources = m_sourceIndexHash.values(fileName);
    source = sources.at(sources.size() - 1);

    if (alIsSource(source) == AL_TRUE) {

        // Loop the source.
        alSourcei(source, AL_LOOPING, AL_TRUE);

        // Check to see if the sound is already looping or not, if not, loop sound.
        ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            alSourcePlay(source);
            return "Looping " + fileName.toStdString();
        }
        return fileName.toStdString() + " is already playing";
    }
    return "Could not find the file " + fileName.toStdString() + " . Maybe it hasn't been loaded.";
}

/**
 * It will be called from JNext JavaScript side with passed string.
 * This method implements the interface for the JavaScript to native binding
 * for invoking native code. This method is triggered when JNext.invoke is
 * called on the JavaScript side with this native objects id.
 */
string PGaudio::InvokeMethod(const string& command)
{

    // parse command and args from string
    int indexOfFirstSpace = command.find_first_of(" ");
    string strCommand = command.substr(0, indexOfFirstSpace);
    string strValue = command.substr(indexOfFirstSpace + 1, command.length());

    // Convert input file name from string to QString
    QString fileName = QString::fromStdString(strValue);

    // Determine which function should be executed

    // Play with given fileName.
    if (strCommand == "play")
        return play(fileName);
 
    // Loop with given fileName.
    if (strCommand == "load") {
        // parse path from strValue, end up with fileAndVoice
        int indexOfSecondSpace = strValue.find_first_of(" ");
        string pathToSounds = strValue.substr(0, indexOfSecondSpace);
        string fileAndVoice = strValue.substr(indexOfSecondSpace + 1, strValue.length());

        // parse file and voices from fileAndVoices
        int indexOfThirdSpace = fileAndVoice.find_first_of(" ");
        string file = fileAndVoice.substr(0, indexOfThirdSpace);
        string voiceString = fileAndVoice.substr(indexOfThirdSpace + 1, fileAndVoice.length());

        // Convert from strings to QString, used as parameters in functions
        QString path = QString::fromStdString(pathToSounds);
        QString name = QString::fromStdString(file);
        int voice = atoi(voiceString.c_str());

        // Preload with given path, fileName, and voice
        return preload(path, name, voice);
    }
    // Loops the source
    if (strCommand == "loop")
        return loop(fileName);

    // Unloads the source
    if (strCommand == "unload")
        return unload(fileName);

    // Stop the source.
    if (strCommand == "stop")
        return stop(fileName);

    // Get duration
    if (strCommand == "getDuration"){
        float result = getDuration(fileName);
        ostringstream buffer;
        buffer << result;
        string str = buffer.str();
        return str;
    }

    return "Command not found, choose either: load, unload, play ,loop, or stop";
}
