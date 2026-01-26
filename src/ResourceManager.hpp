#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class ResourceManager {
public:
    // Singleton pattern - get the single instance
    static ResourceManager& getInstance();

    // Texture loading
    bool loadTexture(const std::string& name, const std::string& path);
    sf::Texture& getTexture(const std::string& name);
    bool hasTexture(const std::string& name) const;

    // Sound loading
    bool loadSoundBuffer(const std::string& name, const std::string& path);
    sf::SoundBuffer& getSoundBuffer(const std::string& name);
    bool hasSoundBuffer(const std::string& name) const;
    std::unique_ptr<sf::Sound> createSound(const std::string& name);

    // Font loading
    bool loadFont(const std::string& name, const std::string& path);
    sf::Font& getFont(const std::string& name);
    bool hasFont(const std::string& name) const;

    // Helper method to try multiple paths
    bool loadTextureFromPaths(const std::string& name, const std::vector<std::string>& paths);
    bool loadSoundBufferFromPaths(const std::string& name, const std::vector<std::string>& paths);
    bool loadFontFromPaths(const std::string& name, const std::vector<std::string>& paths);

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Font> fonts;
};

