#include "../include/AudioEngine.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"

#ifdef ENABLE_AUDIO
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <fstream>
#include <cstring>

namespace Sparky {

#ifdef ENABLE_AUDIO
    // AudioBuffer implementation
    AudioBuffer::AudioBuffer() : bufferId(0), channels(0), sampleRate(0), bitsPerSample(0) {
    }

    AudioBuffer::~AudioBuffer() {
        unload();
    }

    bool AudioBuffer::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading audio file: " + filepath);
        
        // In a real implementation, we would load and decode the audio file
        // For now, we'll create a placeholder implementation
        try {
            // Read file data
            std::vector<char> fileData = FileUtils::readFile(filepath);
            
            // For this example, we'll assume it's a WAV file and parse the header
            // In a real implementation, we would use a library like dr_wav or similar
            
            if (fileData.size() < 44) {
                SPARKY_LOG_ERROR("Audio file too small to be a valid WAV file: " + filepath);
                return false;
            }
            
            // Parse WAV header (simplified)
            // Check RIFF header
            if (std::memcmp(fileData.data(), "RIFF", 4) != 0) {
                SPARKY_LOG_ERROR("Not a valid WAV file: " + filepath);
                return false;
            }
            
            // Get format information
            channels = *reinterpret_cast<int16_t*>(fileData.data() + 22);
            sampleRate = *reinterpret_cast<int32_t*>(fileData.data() + 24);
            bitsPerSample = *reinterpret_cast<int16_t*>(fileData.data() + 34);
            
            // Get audio data (after 44-byte header)
            data.assign(fileData.begin() + 44, fileData.end());
            
            // In a real implementation, we would create an OpenAL buffer and upload the data
            alGenBuffers(1, &bufferId);
            ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
            alBufferData(bufferId, format, data.data(), data.size(), sampleRate);
            
            SPARKY_LOG_DEBUG("Audio file loaded successfully: " + filepath);
            SPARKY_LOG_DEBUG("Channels: " + std::to_string(channels) + 
                           ", Sample Rate: " + std::to_string(sampleRate) + 
                           ", Bits Per Sample: " + std::to_string(bitsPerSample));
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load audio file: " + filepath + " - " + std::string(e.what()));
            return false;
        }
    }

    void AudioBuffer::unload() {
        if (bufferId != 0) {
            alDeleteBuffers(1, &bufferId);
            bufferId = 0;
        }
        data.clear();
    }

    // AudioSource implementation
    AudioSource::AudioSource() : sourceId(0), buffer(nullptr), volume(1.0f), pitch(1.0f), looping(false) {
        alGenSources(1, &sourceId);
    }

    AudioSource::~AudioSource() {
        alDeleteSources(1, &sourceId);
    }

    void AudioSource::setBuffer(AudioBuffer* buffer) {
        this->buffer = buffer;
        if (buffer && sourceId != 0) {
            alSourcei(sourceId, AL_BUFFER, buffer->getBufferId());
        }
    }

    void AudioSource::play() {
        if (sourceId != 0) {
            alSourcePlay(sourceId);
            SPARKY_LOG_DEBUG("Playing audio source");
        }
    }

    void AudioSource::pause() {
        if (sourceId != 0) {
            alSourcePause(sourceId);
            SPARKY_LOG_DEBUG("Pausing audio source");
        }
    }

    void AudioSource::stop() {
        if (sourceId != 0) {
            alSourceStop(sourceId);
            SPARKY_LOG_DEBUG("Stopping audio source");
        }
    }

    void AudioSource::setVolume(float volume) {
        this->volume = volume;
        if (sourceId != 0) {
            alSourcef(sourceId, AL_GAIN, volume);
        }
    }

    void AudioSource::setPitch(float pitch) {
        this->pitch = pitch;
        if (sourceId != 0) {
            alSourcef(sourceId, AL_PITCH, pitch);
        }
    }

    void AudioSource::setPosition(float x, float y, float z) {
        if (sourceId != 0) {
            alSource3f(sourceId, AL_POSITION, x, y, z);
        }
    }

    void AudioSource::setLooping(bool looping) {
        this->looping = looping;
        if (sourceId != 0) {
            alSourcei(sourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
        }
    }

    bool AudioSource::isPlaying() const {
        if (sourceId != 0) {
            ALint state;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
            return state == AL_PLAYING;
        }
        return false;
    }

    bool AudioSource::isPaused() const {
        if (sourceId != 0) {
            ALint state;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
            return state == AL_PAUSED;
        }
        return false;
    }

    // AudioEngine implementation
    AudioEngine::AudioEngine() : initialized(false) {
    }

    AudioEngine::~AudioEngine() {
        shutdown();
    }

    AudioEngine& AudioEngine::getInstance() {
        static AudioEngine instance;
        return instance;
    }

    bool AudioEngine::initialize() {
        if (initialized) {
            return true;
        }

        SPARKY_LOG_INFO("Initializing audio engine...");
        
        // Initialize OpenAL:
        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device) {
            SPARKY_LOG_ERROR("Failed to open audio device");
            return false;
        }

        ALCcontext* context = alcCreateContext(device, nullptr);
        if (!context) {
            SPARKY_LOG_ERROR("Failed to create audio context");
            alcCloseDevice(device);
            return false;
        }

        if (!alcMakeContextCurrent(context)) {
            SPARKY_LOG_ERROR("Failed to make audio context current");
            alcDestroyContext(context);
            alcCloseDevice(device);
            return false;
        }

        initialized = true;
        SPARKY_LOG_INFO("Audio engine initialized successfully");
        return true;
    }

    void AudioEngine::shutdown() {
        if (!initialized) {
            return;
        }

        SPARKY_LOG_INFO("Shutting down audio engine...");

        // Clean up audio sources
        audioSources.clear();

        // Clean up audio buffers
        audioBuffers.clear();

        // Shutdown OpenAL
        ALCcontext* context = alcGetCurrentContext();
        ALCdevice* device = alcGetContextsDevice(context);
        
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);

        initialized = false;
        SPARKY_LOG_INFO("Audio engine shut down");
    }

    AudioBuffer* AudioEngine::loadAudioBuffer(const std::string& name, const std::string& filepath) {
        if (!initialized) {
            SPARKY_LOG_ERROR("Audio engine not initialized");
            return nullptr;
        }

        // Check if buffer already exists
        auto it = audioBuffers.find(name);
        if (it != audioBuffers.end()) {
            return it->second.get();
        }

        // Create new buffer
        auto buffer = std::make_unique<AudioBuffer>();
        if (!buffer->loadFromFile(filepath)) {
            SPARKY_LOG_ERROR("Failed to load audio buffer: " + filepath);
            return nullptr;
        }

        AudioBuffer* bufferPtr = buffer.get();
        audioBuffers[name] = std::move(buffer);
        return bufferPtr;
    }

    AudioSource* AudioEngine::createAudioSource() {
        if (!initialized) {
            SPARKY_LOG_ERROR("Audio engine not initialized");
            return nullptr;
        }

        auto source = std::make_unique<AudioSource>();
        AudioSource* sourcePtr = source.get();
        audioSources.push_back(std::move(source));
        return sourcePtr;
    }

    void AudioEngine::destroyAudioSource(AudioSource* source) {
        if (!initialized || !source) {
            return;
        }

        audioSources.erase(
            std::remove_if(audioSources.begin(), audioSources.end(),
                [source](const std::unique_ptr<AudioSource>& s) {
                    return s.get() == source;
                }),
            audioSources.end()
        );
    }

    void AudioEngine::setListenerPosition(float x, float y, float z) {
        if (initialized) {
            alListener3f(AL_POSITION, x, y, z);
        }
    }

    void AudioEngine::setListenerOrientation(float forwardX, float forwardY, float forwardZ, 
                                           float upX, float upY, float upZ) {
        if (initialized) {
            float orientation[] = {forwardX, forwardY, forwardZ, upX, upY, upZ};
            alListenerfv(AL_ORIENTATION, orientation);
        }
    }

    void AudioEngine::update() {
        // In a more advanced implementation, we might update 3D audio positions,
        // handle streaming audio, etc.
    }
#else
    // Placeholder implementations when audio is disabled
    AudioBuffer::AudioBuffer() : bufferId(0), channels(0), sampleRate(0), bitsPerSample(0) {
    }

    AudioBuffer::~AudioBuffer() {
    }

    bool AudioBuffer::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping load of " + filepath);
        return true;
    }

    void AudioBuffer::unload() {
    }

    AudioSource::AudioSource() : sourceId(0), buffer(nullptr), volume(1.0f), pitch(1.0f), looping(false) {
    }

    AudioSource::~AudioSource() {
    }

    void AudioSource::setBuffer(AudioBuffer* buffer) {
    }

    void AudioSource::play() {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping play");
    }

    void AudioSource::pause() {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping pause");
    }

    void AudioSource::stop() {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping stop");
    }

    void AudioSource::setVolume(float volume) {
        this->volume = volume;
    }

    void AudioSource::setPitch(float pitch) {
        this->pitch = pitch;
    }

    void AudioSource::setPosition(float x, float y, float z) {
    }

    void AudioSource::setLooping(bool looping) {
        this->looping = looping;
    }

    bool AudioSource::isPlaying() const {
        return false;
    }

    bool AudioSource::isPaused() const {
        return false;
    }

    AudioEngine::AudioEngine() : initialized(false) {
    }

    AudioEngine::~AudioEngine() {
    }

    AudioEngine& AudioEngine::getInstance() {
        static AudioEngine instance;
        return instance;
    }

    bool AudioEngine::initialize() {
        SPARKY_LOG_INFO("Audio engine disabled");
        return true;
    }

    void AudioEngine::shutdown() {
    }

    AudioBuffer* AudioEngine::loadAudioBuffer(const std::string& name, const std::string& filepath) {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping load of " + filepath);
        return nullptr;
    }

    AudioSource* AudioEngine::createAudioSource() {
        SPARKY_LOG_DEBUG("Audio disabled: Skipping create audio source");
        return nullptr;
    }

    void AudioEngine::destroyAudioSource(AudioSource* source) {
    }

    void AudioEngine::setListenerPosition(float x, float y, float z) {
    }

    void AudioEngine::setListenerOrientation(float forwardX, float forwardY, float forwardZ, 
                                           float upX, float upY, float upZ) {
    }

    void AudioEngine::update() {
    }
#endif
}