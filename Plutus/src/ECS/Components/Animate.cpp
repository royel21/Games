#include "Animate.h"
#include "Serialize/Serialize.h"

namespace Plutus
{
    Animation::Animation() : startFrame(0), aninSpeed(0.166f) {}

    Animation::Animation(unsigned int _startFrame, unsigned int _numFrames, int _aninSpeed)
        : startFrame(_startFrame), aninSpeed(1.0f / (_aninSpeed / 16.66f))
    {
    }

    Animate::Animate(float _aninTime, const std::string aninId, Animation anin)
    {
        aninTime = _aninTime;
        animations[aninId] = anin;
        currentAnin = &anin;
    }

    void Animate::AddAnimation(const std::string id, const Animation &anin)
    {
        animations[id] = anin;
    }

    void Animate::PlayAnimation(const std::string &aninName, bool _loop)
    {
        currentAnin = &animations[aninName];
        loop = _loop;
    }

    void Animate::serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Animate");
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
} // namespace Plutus