#include "ResourceManager.hpp"
#include <vector>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture texture;
    if (texture.loadFromFile(path)) {
        textures[name] = std::move(texture);
        return true;
    }
    return false;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    return textures.at(name);
}

bool ResourceManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}

bool ResourceManager::loadSoundBuffer(const std::string& name, const std::string& path) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(path)) {
        soundBuffers[name] = std::move(buffer);
        return true;
    }
    return false;
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) {
    return soundBuffers.at(name);
}

bool ResourceManager::hasSoundBuffer(const std::string& name) const {
    return soundBuffers.find(name) != soundBuffers.end();
}

std::unique_ptr<sf::Sound> ResourceManager::createSound(const std::string& name) {
    if (hasSoundBuffer(name)) {
        return std::make_unique<sf::Sound>(getSoundBuffer(name));
    }
    return nullptr;
}

bool ResourceManager::loadFont(const std::string& name, const std::string& path) {
    sf::Font font;
    if (font.openFromFile(path)) {
        fonts[name] = std::move(font);
        return true;
    }
    return false;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    return fonts.at(name);
}

bool ResourceManager::hasFont(const std::string& name) const {
    return fonts.find(name) != fonts.end();
}

bool ResourceManager::loadTextureFromPaths(const std::string& name, const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        if (loadTexture(name, path)) {
            return true;
        }
    }
    return false;
}

bool ResourceManager::loadSoundBufferFromPaths(const std::string& name, const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        if (loadSoundBuffer(name, path)) {
            return true;
        }
    }
    return false;
}

bool ResourceManager::loadFontFromPaths(const std::string& name, const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
        if (loadFont(name, path)) {
            return true;
        }
    }
    return false;
}

