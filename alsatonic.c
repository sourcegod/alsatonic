/*
 * File: alsatonic.c
 * Tone generator from alsa audio api
 * Author: Coolbrother
 * Date: 24/12/2020
 * */

#include <alsa/asoundlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN 22050 // 48000
#define DEFAULT_FREQ 440
#define DEFAULT_DURATION 1
float g_buffer[BUFFER_LEN];
snd_pcm_t *g_handle;
snd_pcm_sframes_t g_frames;

float* genTone(float freq) {
    // must create a pointer to return buffer
    // float *buf = malloc(sizeof(float) * BUFFER_LEN);
    for (int i=0; i<BUFFER_LEN ; i++) {
		    g_buffer[i] = sin(2*M_PI * freq / BUFFER_LEN * i); // Creating the sinusoid
    }

      return g_buffer;
}
//-----------------------------------------

int openDevice() {
    // open sound device and set params
 	  const static char *device = "default";
	  snd_output_t *output = NULL;
   int err;

    if ((err = snd_pcm_open(&g_handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      fprintf(stderr, "AlsaTonic: Playback open error: %s\n", snd_strerror(err));
      return EXIT_FAILURE;
    }

    if ((err = snd_pcm_set_params(g_handle,
        SND_PCM_FORMAT_FLOAT,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1,
        BUFFER_LEN,
        1,
        500000)) < 0) {	 
      fprintf(stderr, "AlsaTonic: Playback open error: %s\n", snd_strerror(err));
      return EXIT_FAILURE;
    }

}
//-----------------------------------------

void closeDevice() {
    // closing sound device
	  snd_pcm_close(g_handle);

}
//-----------------------------------------

void writeBuf(float freq, int duration) {
  float* buf;
  for (int i=0 ; i < duration ; i++) {
    buf = genTone(freq);
		g_frames = snd_pcm_writei(g_handle, buf, BUFFER_LEN); // Sending the sound
    // printf("nbFrames: %d", g_frames);
  }

}
//-----------------------------------------

void playFreq(float freq, float dur) {
    // playing one freq
    writeBuf(freq, dur);	
}
//-----------------------------------------

void playSeq(float freq, float dur, int start, int stop, float step) {
    // playing sequence freq
    int iStep = (int)(step);
    for (int i=start; i<stop; i += iStep){
        writeBuf(freq, dur);	
        freq += step;
    }

}
//-----------------------------------------


int main(int argc, char *argv[]) {
    int err;
    int def_start =0;
    int def_stop=1;
    float def_step =1;
    int isSeq =0;
    if (argc > 3) isSeq =1;
    
    // SINE WAVE
    float freq = (argc > 1) ? atof(argv[1]) : DEFAULT_FREQ;
    if (freq == 0) {
      fprintf(stderr, "AlsaTonic: Invalid frequency.\n");
      return EXIT_FAILURE;
    }

    float duration = (argc > 2) ? atof(argv[2]) : DEFAULT_DURATION;
    if (duration == 0) {
      fprintf(stderr, "AlsaTonic: Invalid duration.\n");
      return EXIT_FAILURE;
    }
    
    int start = (argc > 3) ? strtol(argv[3], NULL, 10) : def_start;
    int stop = (argc > 4) ? strtol(argv[4], NULL, 10) : def_stop;
    float step = (argc > 5) ? atof(argv[5]) : def_step;

    if (err = openDevice()) {
        return EXIT_FAILURE;
    }
    
    if (!isSeq) {
        printf("Playing freq, Sine tone at %.3fHz during %.2f seconds.\n", freq, duration);
        playFreq(freq, duration);
    } else {
        printf("Playing seq, Sine tone at %.3fHz during %.2f seconds.\n", freq, duration);
        playSeq(freq, duration, start, stop, step);
    }
    closeDevice();

    return EXIT_SUCCESS;
}
//-----------------------------------------
