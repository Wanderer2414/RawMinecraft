#include "Font.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "stb_truetype.h"

namespace MyBase {
    Font::Font(const std::string& source) {
        if (source.size()) loadFont(source);
    }
    Font::~Font() {
        glDeleteTextures(1, &__textmap);
    }
    void Font::loadFont(const std::string& source) {
        std::ifstream fstream(source);
        if (!fstream.is_open()) {
            std::cerr << "Cant load font: " << source << std::endl;
            exit(0);
        }
        else std::cout << "Load font successful: " << source << std::endl;
        fstream.seekg(0, std::ios::end);
        int sz = fstream.tellg();
        fstream.seekg(0);
        unsigned char data[1<<20];
        fstream.read((char*)&data, sz);
        unsigned char bitmap[512*512];
        stbtt_BakeFontBitmap(data, 0, 32, bitmap, 512, 512, 32, 96, __data);

        glGenTextures(1, &__textmap);
        glBindTexture(GL_TEXTURE_2D, __textmap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE,  bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void Font::Bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __textmap);
    }
    glm::vec2 Font::MeasureText(const std::string& text) const {
        auto map = __utf_to_codepoint(text);
        glm::vec2 size = {0, 0};
        float y = 0;
        for (int i = 0; i<map.size(); i++) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(__data, 512, 512, map[i]-32, &size.x, &y, &q, 1);
            size.y = std::max(size.y, (q.y0-q.y1)/512/32);
        }
        return size;
    }
    char* Font::getBuffer(const std::string& text, glm::vec2& size) const{
        auto map = __utf_to_codepoint(text);
        size = {0, 0};
        float x = 0, y = 0, minY = 0;
        char* buffer = new char[text.size()*(sizeof(glm::vec2)*2)*6];
        glm::vec2* positions = (glm::vec2*)buffer;
        glm::vec2* uvs = (glm::vec2*)(buffer + text.size()*sizeof(glm::vec2)*6);
        for (int i = 0; i<map.size(); i++) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(__data, 512, 512, map[i]-32, &x, &y, &q, 1);
            q.x0=q.x0/512; q.y0 = - (q.y0+32.f)/512/32;
            q.x1=q.x1/512; q.y1 = - (q.y1+32.f)/512/32;
            size.y = std::max(size.y, q.y0-q.y1);
            minY = std::min(minY, q.y1);
            positions[6*i] = {q.x0, q.y0};   uvs[6*i] = {q.s0, q.t0};
            positions[6*i+1] = {q.x0, q.y1}; uvs[6*i+1] = {q.s0, q.t1};
            positions[6*i+2] = {q.x1, q.y1}; uvs[6*i+2] = {q.s1, q.t1};
            positions[6*i+3] = {q.x0, q.y0}; uvs[6*i+3] = {q.s0, q.t0};
            positions[6*i+4] = {q.x1, q.y1}; uvs[6*i+4] = {q.s1, q.t1};
            positions[6*i+5] = {q.x1, q.y0}; uvs[6*i+5] = {q.s1, q.t0};
        }
        for (int i = 0; i<map.size()*6; i++) positions[i].y -= minY;
        size.x = x/512;
        return buffer;
    }
    std::vector<int> Font::__utf_to_codepoint(const std::string& text) const {
        std::vector<int> ans;
        for (int i = 0; i<text.size();) {        
            unsigned char c = text[i];
            if (c < 128) {
                i++;
                ans.push_back(c);
            }
            else {
                ans.push_back('?');
                i+=1;
            }
        }        
        return ans;
    }
};