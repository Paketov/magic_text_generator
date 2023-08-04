#include <stdio.h>
#include <intrin.h>
#include <windows.h>
#include <stdint.h>


typedef struct DICTIONARY_WORD {
	wchar_t* word;					//?????
	int* next_words_indexes;		//??????? ????????? ????
	int* next_words_rating;
	unsigned next_words_count;
};


#pragma intrinsic(__rdtsc)


float MyRandomFunc() {
	static int j = 0;
	if (j == 0) {
		j = 1;
		unsigned __int64 v = __rdtsc();
		Sleep(0);
		for (int i = 0; i < 100; i++) {
			unsigned __int64 a = __rdtsc();
			FILE* f = fopen("C:\\Windows\\Temp\\rdtsc_rand", "wb");
			fwrite(&a, sizeof(a), 1, f);
			fclose(f);
			Sleep(0);
			unsigned __int64 b = __rdtsc();
			v = v * 214013ULL;
			v += (b - a);
		}
		srand(unsigned int(v % 0xfffffffull));
	}
	return float(2) * ((float)rand() / ((float)RAND_MAX)) - 1;//float(2) * (float(v % 0xFFFFFFULL) / float(0xFFFFFFULL)) - 1;  //float(2) * ((float)rand() / ((float)RAND_MAX)) - 1;
}
int MyRandomFunc2() {
	static int j = 0;
	j = 1;
	unsigned __int64 v = __rdtsc();
	Sleep(0);
	for (int i = 0; i < 100; i++) {
		unsigned __int64 a = __rdtsc();
		FILE* f = fopen("C:\\Windows\\Temp\\rdtsc_rand", "wb");
		fwrite(&a, sizeof(a), 1, f);
		fclose(f);
		Sleep(0);
		unsigned __int64 b = __rdtsc();
		v = v * 214013ULL;
		v += (b - a);
	}
	return v & 0x7fffffff;// % 0xfffffffull;
}


int MyRandomFunc3() {
	static int j = 0;
	if (j == 0) {
		j = 1;
		unsigned __int64 v = __rdtsc();
		Sleep(0);
		for (int i = 0; i < 100; i++) {
			unsigned __int64 a = __rdtsc();
			FILE* f = fopen("C:\\Windows\\Temp\\rdtsc_rand", "wb");
			fwrite(&a, sizeof(a), 1, f);
			fclose(f);
			Sleep(0);
			unsigned __int64 b = __rdtsc();
			v = v * 214013ULL;
			v += (b - a);
		}
		srand(unsigned int(v % 0xfffffffull));
	}
	int r = rand() * 214013 + rand();
	if ((r % 7) == 0) {
		unsigned __int64 v = __rdtsc();
		Sleep(0);
		for (int i = 0; i < 100; i++) {
			unsigned __int64 a = __rdtsc();
			FILE* f = fopen("C:\\Windows\\Temp\\rdtsc_rand", "wb");
			fwrite(&a, sizeof(a), 1, f);
			fclose(f);
			Sleep(0);
			unsigned __int64 b = __rdtsc();
			v = v * 214013ULL;
			v += (b - a);
		}
		srand(unsigned int(v % 0xfffffffull));
	}
	return r;
}

int MyRandomFunc4() {
		unsigned __int64 v = __rdtsc();
		Sleep(0);
		for (int i = 0; i < 3; i++) {
			unsigned __int64 a = __rdtsc();
			FILE* f = fopen("C:\\Windows\\Temp\\rdtsc_rand", "wb");
			fwrite(&a, sizeof(a), 1, f);
			fclose(f);
			Sleep(0);
			unsigned __int64 b = __rdtsc();
			v = v * 214013ULL;
			v += (b - a);
		}
		return int(v % 0x7ffffffull);
}



uint32_t StrCharUtf16ToLower(uint32_t r) {
	wchar_t Buf[3];
	if (r > 0xffff) {
		Buf[0] = r >> 16;
		Buf[1] = r & 0xffff;
		Buf[2] = L'\0';
	}
	else {
		Buf[0] = r;
		Buf[1] = L'\0';
	}
	CharLowerW(Buf);
	if (Buf[1])
		return ((uint32_t)Buf[0] << 16) | Buf[1];
	else
		return Buf[0];
}

bool StrCharUtf16IsAlpha(uint32_t r) {
	return IsCharAlphaW(r) == TRUE;
}

void PrintWords(DICTIONARY_WORD** words) {
	
}

void FillWordsDictionary(DICTIONARY_WORD*** _words, uint32_t* _count_words, wchar_t* file_name) {
	DICTIONARY_WORD** words = *_words;
	uint32_t count_words = *_count_words;
	int prev_word = -1;
	FILE* input_words = _wfopen(file_name, L"rb");
	if (input_words == NULL) {
		wprintf(L"Invalid book path (%s)\n", file_name);
		return;
	}
	fseek(input_words, 0, SEEK_END);
	long file_size = ftell(input_words);
	fseek(input_words, 0, SEEK_SET);
	void* text_utf8 = malloc(file_size);
	wchar_t* text_utf16 = (wchar_t*)malloc(file_size * 3);
	fread(text_utf8, 1, file_size, input_words);
	fclose(input_words);
	int text_w16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)text_utf8, file_size, (LPWSTR)text_utf16, file_size * 3);
	free(text_utf8);

	for (wchar_t* c = text_utf16, *m = c + text_w16_size; c< m; ) {
		int word_size = 0;
		for (; (c < m) && !StrCharUtf16IsAlpha(*c); c++) {
			switch (*c) {
			case L'.':
			case L';':
			case L':':
			case L'!':
			case L'?':
			case L'(':
			case L')':
			case L'[':
			case L']':
			case L'<':
			case L'>':
				prev_word = -1;
			}
		}
		if (c >= m)
			break;

		for (wchar_t* ct = c; (ct < m) && StrCharUtf16IsAlpha(*ct); ct++) {
			word_size++;
		}
		wchar_t* w = (wchar_t*)malloc(word_size * sizeof(wchar_t) + sizeof(wchar_t) * 2);


		for (wchar_t *m = c + word_size, *wt = w; c < m; c++, wt++) {
			*wt = StrCharUtf16ToLower(*c);
		}
		w[word_size] = L'\0';
		for (int i = 0; i < count_words; i++) {
			if (wcscmp(words[i]->word, w) == 0) {
				if (prev_word != -1) {
					for (int j = 0; j < words[prev_word]->next_words_count; j++) {
						if (words[prev_word]->next_words_indexes[j] == i) {
							words[prev_word]->next_words_rating[j]++;
							prev_word = i;
							free(w);
							goto lblContinue;
						}
					}
					words[prev_word]->next_words_count++;
					words[prev_word]->next_words_indexes = (int*)realloc(words[prev_word]->next_words_indexes, sizeof(int)*words[prev_word]->next_words_count);
					words[prev_word]->next_words_rating = (int*)realloc(words[prev_word]->next_words_rating, sizeof(int)*words[prev_word]->next_words_count);
					words[prev_word]->next_words_indexes[words[prev_word]->next_words_count - 1] = i;
					words[prev_word]->next_words_rating[words[prev_word]->next_words_count - 1] = 0;
				}
				prev_word = i;
				free(w);
				goto lblContinue;
			}
		}
		count_words++;
		words = (DICTIONARY_WORD**)realloc(words, count_words * sizeof(DICTIONARY_WORD*));
		DICTIONARY_WORD* dw = (DICTIONARY_WORD*)malloc(sizeof(DICTIONARY_WORD));
		words[count_words - 1] = dw;
		dw->word = w;
		dw->next_words_count = 0;
		dw->next_words_indexes = NULL;
		dw->next_words_rating = NULL;
		if (prev_word != -1) {
			words[prev_word]->next_words_count++;
			words[prev_word]->next_words_indexes = (int*)realloc(words[prev_word]->next_words_indexes, sizeof(int)*words[prev_word]->next_words_count);
			words[prev_word]->next_words_rating = (int*)realloc(words[prev_word]->next_words_rating, sizeof(int)*words[prev_word]->next_words_count);
			words[prev_word]->next_words_indexes[words[prev_word]->next_words_count - 1] = count_words - 1;
			words[prev_word]->next_words_rating[words[prev_word]->next_words_count - 1] = 0;
		}
		prev_word = count_words - 1;
	lblContinue:;
	}
	free(text_utf16);

	*_count_words = count_words;
	*_words = words;
}





int wmain(int argc, wchar_t *argv[]) {
	DICTIONARY_WORD** words = NULL;
	uint32_t count_words = 0;
	DWORD chars_written = 0;
	if (argc < 2) {
		wprintf(L"Usage: ConsoleApplication1.exe <book 1 path> [<book 2 path> ...]\n");
		return 1;
	}
	for (int i = 1; i < argc; i++) {
		FillWordsDictionary(&words, &count_words, argv[i]);
	}
	int prev_word = -1;
	for (;;) {
		Sleep(600);
		int word_index = MyRandomFunc4();
		DICTIONARY_WORD* word;
		if (prev_word == -1) {
			prev_word = word_index % count_words;
			word = words[prev_word];
		} else {
			if (words[prev_word]->next_words_count == 0) {
				prev_word = -1;
				WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L".", 1, &chars_written, NULL);
				continue;
			}
			int t = words[prev_word]->next_words_indexes[word_index % words[prev_word]->next_words_count];
			if (t == prev_word) {
				prev_word = -1;
				continue;
			}
			prev_word = t;
			word = words[prev_word];
		}
		
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), word->word, wcslen(word->word), &chars_written, NULL);
		WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L" ", 1, &chars_written, NULL);
		//fwprintf(stdout, L"%s ", word->word);

	}

	return 0;
}

