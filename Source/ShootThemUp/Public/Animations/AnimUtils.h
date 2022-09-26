#pragma once

class AnimUtils {
public:
    
    template<class T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation) {
        if (!Animation) return nullptr;
    
        const auto NotifyEvents = Animation->Notifies;

        for (const auto& NotifyEvent: NotifyEvents) {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify) { // if Succesfull
                return AnimNotify;
            }
        }
        return nullptr;
    }
};
