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
#include <time.h>
#define BUF_LEN 48000
#define DEFAULT_FREQ 440
#define DEFAULT_DURATION 1
float g_buffer[BUF_LEN];
snd_pcm_t *g_handle;
snd_pcm_sframes_t g_frames;
int channels =1;
snd_pcm_format_t format = SND_PCM_FORMAT_FLOAT;
int rate = 48000;


float* genTone(float freq) {
    // must create a pointer to return buffer
    // float *buf = malloc(sizeof(float) * BUF_LEN);
    float vol =1;
    float t = 2*M_PI*freq/(rate*channels);
    // int nbSamples = rate * channels * dur;
    // printf("nbSamples: %d\n", nbSamples);
    int maxSamp = 32767;
    float* buf = g_buffer;
    for (int i=0; i< BUF_LEN; i++) {
        // val = (int)maxSamp*vol*sin(t*i);
        g_buffer[i] = sin(t*i);
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
        format,
        // SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        channels,
        // BUF_LEN,
        rate,
        1, /* period */
        500000)) < 0) {	 /* latency: 0.5sec */ 
      fprintf(stderr, "AlsaTonic: Playback open error: %s\n", snd_strerror(err));
      return EXIT_FAILURE;
    }

}
//-----------------------------------------

void closeDevice() {
    // closing sound device
    // necessary to flush and send short sample
    snd_pcm_drain(g_handle);
	  snd_pcm_close(g_handle);

}
//-----------------------------------------

void writeBuf(float* buf, int nbFrames, int nbTimes) {
  for (int i=0; i < nbTimes; i++) {
      // Sending the sound
      g_frames += snd_pcm_writei(g_handle, buf, nbFrames);
  }
  printf("WriteBuf nbFrames: %d\n", g_frames);

}
//-----------------------------------------

void writeAudio(unsigned int nbFrames) {
      // Sending the sound
      int frames = snd_pcm_writei(g_handle, g_buffer, nbFrames);
}
//-----------------------------------------

void playFreq(float freq, float dur) {
    // playing one freq
    //
    float* buf;
    int nbSamples = rate * channels * dur;
    int nbTimes = nbSamples / BUF_LEN;
    int restFrames = nbSamples % BUF_LEN;
    printf("restFrames: %d\n", restFrames);
    /*
    for (int i=0; i < nbBuf; i++) {
        buf = genTone(freq);
        writeAudio(buf, BUF_LEN);
    }
    printf("nbFrames: %d\n", g_frames);
    */

    if (nbSamples >0) {
        buf = genTone(freq);
        if (nbTimes >0) {
            writeBuf(buf, BUF_LEN, nbTimes);	
        }
        if (restFrames > 0) {
            writeBuf(buf, restFrames, 1);
        }
    
    }

}
//-----------------------------------------

void playSeq(float freq, float dur, int start, int stop, float step) {
    // playing sequence freq
    int iStep = (int)(step);
    for (int i=start; i<stop; i += iStep){
        playFreq(freq, dur);	
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
