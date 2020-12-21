#ifndef __TIMER_H__
#define __TIMER_H__

namespace Plutus
{
    class T_Timer
    {
    public:
        ~T_Timer();
        //remove the copy constructor
        T_Timer(const T_Timer &) = delete;
        //Override the assign operator
        T_Timer &operator=(const T_Timer &) = delete;
        //Return the only reference to the timer
        static T_Timer &Ref()
        {
            static T_Timer reference;
            return reference;
        }

        void Tick();
        void Initialize();
        inline float DeltaTime() { return deltaTime; }

    private:
        T_Timer();

    private:
        float lastTime, deltaTime;
    };
    static T_Timer &Timer = T_Timer::Ref();
} // namespace Plutus

#endif // __TIMER_H__