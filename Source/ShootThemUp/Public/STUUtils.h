#pragma once

class STUUtils {
public:

    template<class T>
    static T* GetWeaponComponent(APawn* PlayerPawn) {
        if (!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);;
    }
    
};
