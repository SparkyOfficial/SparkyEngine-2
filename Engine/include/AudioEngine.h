#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Sparky {
    class AudioBuffer {
    public:
        AudioBuffer();
        ~AudioBuffer();

        bool loadFromFile(const std::string& filepath);
        void unload();

        // Getters
        unsigned int getBufferId() const { return bufferId; }
        int getChannels() const { return channels; }
        int getSampleRate() const { return sampleRate; }
        int getBitsPerSample() const { return bitsPerSample; }

    private:
        unsigned int bufferId;
        int channels;
        int sampleRate;
        int bitsPerSample;
        std::vector<char> data;
    };

    class AudioSource {
    public:
        AudioSource();
        ~AudioSource();

        void setBuffer(AudioBuffer* buffer);
        void play();
        void pause();
        void stop();
        void setVolume(float volume);
        void setPitch(float pitch);
        void setPosition(float x, float y, float z);
        void setLooping(bool looping);

        // Getters
        bool isPlaying() const;
        bool isPaused() const;
        float getVolume() const { return volume; }
        float getPitch() const { return pitch; }

    private:
        unsigned int sourceId;
        AudioBuffer* buffer;
        float volume;
        float pitch;
        bool looping;
    };

    class AudioEngine {
    public:
        static AudioEngine& getInstance();

        bool initialize();
        void shutdown();

        AudioBuffer* loadAudioBuffer(const std::string& name, const std::string& filepath);
        AudioSource* createAudioSource();
        void destroyAudioSource(AudioSource* source);

        void setListenerPosition(float x, float y, float z);
        void setListenerOrientation(float forwardX, float forwardY, float forwardZ, 
                                  float upX, float upY, float upZ);

        void update();

    private:
        AudioEngine();
        ~AudioEngine();

        bool initialized;
        std::unordered_map<std::string, std::unique_ptr<AudioBuffer>> audioBuffers;
        std::vector<std::unique_ptr<AudioSource>> audioSources;
    };
}