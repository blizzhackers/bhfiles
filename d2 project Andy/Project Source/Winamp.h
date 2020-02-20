#define Winamp (FindWindowA("Winamp v1.x", NULL))

LRESULT CurrentTime();
LRESULT MaxTime();
LPSTR CurrentSong();
VOID Rewind();
VOID FastForward();
VOID LowerVolume();
VOID HigherVolume();
VOID NextSong();
VOID Stop();
VOID Pause();
VOID Play();
VOID PreviousSong();