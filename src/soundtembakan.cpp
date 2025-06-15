#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <windows.h> // Diperlukan untuk PlaySound
#include <mmsystem.h> // Diperlukan untuk PlaySound

// Fungsi untuk memutar suara tembak
void playShootSound() {
    PlaySound(TEXT("sound_effects/Shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

// Fungsi untuk memutar suara ketika peluru mengenai target
void playHitSound() {
    PlaySound(TEXT("sound_effects/Hit.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

// Fungsi untuk memutar suara ketika roket menabrak
void playExplodeSound() {
    PlaySound(TEXT("sound_effects/Explode.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

#endif // SOUND_MANAGER_H 