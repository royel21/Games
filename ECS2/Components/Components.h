#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "Assets/AssetManager.h"
#include "Animation.h"
#include "Serialize/Serialize.h"

namespace Plutus
{
    class Renderable2D;

    struct Tag
    {
        Tag() = default;
        Tag(const std::string &_tag) : TagName(_tag) {}
        std::string TagName;
        void Serialize(Serializer &serializer)
        {
            auto writer = serializer.getWriter();
            writer->String("Tag");
            writer->String(TagName.c_str());
        }
    };

    struct Transform
    {
        glm::vec2 position;
        glm::vec2 size;
        int rotation;
        Transform() = default;

        Transform(float x, float y, int w, int h, int r = 0)
        {
            position = glm::vec2(x, y);
            size = glm::vec2(w, h);
            rotation = r;
        }

        Transform(const glm::vec2 &pos, glm::vec2 _size, int r = 0)
        {
            position = pos;
            size = size;
            rotation = r;
        }
        void Serialize(Serializer &serializer)
        {
            auto writer = serializer.getWriter();
            writer->String("Transform");
            writer->StartObject();
            {
                writer->String("x");
                writer->Int(position.x);
                writer->String("y");
                writer->Int(position.y);
                writer->String("w");
                writer->Int(size.x);
                writer->String("h");
                writer->Int(size.y);
            }
            writer->EndObject();
        }
    };

    struct Animate
    {

        bool loop = false;
        float aninTime = 0;
        TileSet *mTileset = nullptr;
        uint32_t currentFrame = 0;
        Animation *currentAnin = nullptr;
        std::map<std::string, Animation> animations;

        Animate() = default;

        Animate(float _aninTime, const std::string aninId, Animation anin) : aninTime(_aninTime)
        {
            animations[aninId] = anin;
            currentAnin = &anin;
        }

        void AddAnimation(const std::string id, const Animation &anin)
        {
            animations[id] = anin;
        }

        void PlayAnimation(const std::string &aninName, bool _loop = false)
        {
            currentAnin = &animations[aninName];
            loop = _loop;
        }
        void Serialize(Serializer &serializer)
        {
            auto writer = serializer.getWriter();
            writer->String("Animate");
            writer->StartObject();
            {
                writer->String("tileset");
                writer->String(mTileset->name.c_str());
                writer->String("aninTime");
                writer->Int(aninTime);
                writer->String("animations");
                writer->StartArray();
                for (auto ani : animations)
                {
                    writer->StartObject();
                    writer->String("name");
                    writer->String(ani.first.c_str());
                    writer->String("textureId");
                    writer->String(ani.second.textureId.c_str());
                    writer->String("startFrame");
                    writer->Int(ani.second.startFrame);
                    writer->String("aninSpeed");
                    writer->Int(ani.second.aninSpeed);
                    writer->String("numFrames");

                    writer->StartArray();
                    for (auto frame : ani.second.frames)
                    {
                        writer->Int(frame);
                    }
                    writer->EndArray();

                    writer->EndObject();
                }
                writer->EndArray();
            }
            writer->EndObject();
        }
        // void setAnimationSpeed(int speed) { currentAnin->aninSpeed = speed; }
    };

} // namespace Plutus