#include <alsa/asoundlib.h> // для установки необходимо скачать библиотеку sudo apt install libasound2-dev
#include <cmath>
#include <iostream>
#include <vector>

#define AMPLITUDE 30000
// Компиляция кода: g++ your_program.cpp -o your_program -lportaudio
// Функция для генерации звукового сигнала заданной частоты
std::vector<int16_t> generateSignal(double frequency, double duration, unsigned int sampleRate) {
    std::vector<int16_t> signal;
    
    const double twoPi = 2 * 3.141593;
    
	//static_cast - выполняет неполиморфное приведение типов. Необходим для дискретного преобразования
    unsigned int numSamples = static_cast<unsigned int>(duration * sampleRate); // высичляем количество дискретных точек
    for (unsigned int i = 0; i < numSamples; ++i) {
        double time = static_cast<double>(i) / sampleRate;
        
        // int16_t sample = static_cast<int16_t>(AMPLITUDE * std::sin(twoPi * frequency * time)); // вычисляем амплитуду сигнала в 16-ти битной кодировке
        int16_t sample = static_cast<int16_t>(AMPLITUDE * sin(twoPi * frequency * time) + 0.5 * sin(2 * twoPi * frequency * time)); // звук, более похожий на фортепианный
        signal.push_back(sample); // заполняем вектор уровня звука по времени
    }

    return signal; // возвращаем вектор
}

int main() {
    const unsigned int sampleRate = 44100; // Частота дискретизации в Гц

    // Maksim song:
    float frequencies[] = {110, 440, 440, 392, 440, 466.16, 440, 466.16, 349.23,392, 392, 392, 349.23, 392, 440, 392, 440, 329.63, 349.23}; // первая частота для паузы
    unsigned int delays[] = {1, 1, 1, 1, 5, 2, 1, 2, 1, 2, 1, 1, 1, 5, 2, 1, 2, 1, 2}; // первая длительность для паузы
    
	delays[sizeof(delays)] = '\n';
	frequencies[sizeof(frequencies)] = '\n';
    // Инициализация ALSA
    snd_pcm_t *pcm_handle; // refers to the PCM (Pulse Code Modulation) interface in the ALSA (Advanced Linux Sound Architecture) library.
    snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0); // a function used in audio programming to create a handle and establish a connection to the audio interface for sound playback. snd_pcm_open(handle, card, device, mode)
    snd_pcm_set_params(pcm_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, sampleRate, 1, 500000); // refers to parameters related to the configuration of the ALSA:

//	u8 num_chan;	/* 1=Mono, 2=Stereo */
//	u8 pcm_wd_sz;	/* 16/24 - bit*/
//	u8 use_offload_path;	/*	0-PCM using period elpased & ALSA interfaces
//								1-PCM stream via compressed interface  */
//	u8 reserved2;
//	u32 sfreq;    /* Sampling rate in Hz */
//	u8 channel_map[8];

	std::vector<int16_t> signal;
	snd_pcm_sframes_t frames_written;
	
	// play song:
	for(auto i = 1; i < sizeof(delays); i++) {
		// генерируем ноту заданной длительности
		signal = generateSignal(frequencies[i], 0.4*delays[i], sampleRate); // (частота сигнала, длительность, частота дискретизации)
	    // Отправка сигнала на звуковое устройство
	    // signal.data() - Возвращает прямой указатель на массив памяти, используемый вектором внутри для хранения принадлежащих ему элементов.
	    frames_written = snd_pcm_writei(pcm_handle, signal.data(), signal.size());

		// пауза
	    signal = generateSignal(frequencies[0], 0.05*delays[0], sampleRate); // (частота сигнала, длительность, частота дискретизации)
	    frames_written = snd_pcm_writei(pcm_handle, signal.data(), signal.size());
	    	    
		// печать ошибки и вывод кода ошибки
		
	    if (frames_written < 0) {
	        std::cerr << "Error writing to PCM device: " << snd_strerror(frames_written) << std::endl; 
	    }
	}
    // Закрытие устройства ALSA
    snd_pcm_close(pcm_handle);

    return 0;
}
