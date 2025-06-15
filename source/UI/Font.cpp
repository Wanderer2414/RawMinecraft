#include "Font.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "stb_truetype.h"
#include <GL/gl.h>
#include <GL/glext.h>

namespace MyBase3D {
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
    void Font::DrawText(const std::string& text, const glm::vec2& position) {
        glUseProgram(ShaderStorage::Default->getFontShader());
        auto map = __utf8_to_codepoint(text);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __textmap);
        
        float x = position.x, y = position.y;
        float color[] = {0, 1, 0};
        GLuint COLOR;
        glGenBuffers(1, &COLOR);
        glBindBuffer(GL_UNIFORM_BUFFER, COLOR);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, COLOR);
        for (int i = 0; i<map.size(); i++) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(__data, 512, 512, map[i]-32, &x, &y, &q, 1);
            q.x0/=512; q.y0 = - q.y0/512;
            q.x1/=512; q.y1 = - q.y1/512;
            float tmp[] = {q.x0, q.y0, q.s0, q.t0, q.x0, q.y1, q.s0, q.t1, q.x1, q.y1, q.s1, q.t1, q.x1, q.y0, q.s1, q.t0};
            unsigned int indices[] = {0, 1, 3, 3, 1, 2};
            GLuint VAO, VBO, IDS;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
    
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(tmp), &tmp, GL_STATIC_DRAW);
    
            glGenBuffers(1, &IDS);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IDS);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
    
            glEnableVertexArrayAttrib(VAO, 0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
            glEnableVertexArrayAttrib(VAO, 1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
            
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &IDS);
        }
    }
    std::vector<int> Font::__utf8_to_codepoint(const std::string& text) {
        std::vector<int> ans;
        for (int i = 0; i<text.size();) {        
            unsigned char c = text[i];
            if (c < 128) {
                i++;
                ans.push_back(c);
            } else if ((c >> 5) == 0x6) {
                i+=2;
                ans.push_back(((text[i] & 0x1F) << 6) | (text[i+1] & 0x3F));
            } else if ((c >> 4) == 0xE) {
                i+=3;
                ans.push_back(((text[0] & 0x0F) << 12) | ((text[1] & 0x3F) << 6) | (text[2] & 0x3F));
            } else if ((c >> 3) == 0x1E) {
                i+=4;
                ans.push_back(((text[0] & 0x07) << 18) |
                       ((text[1] & 0x3F) << 12) |
                       ((text[2] & 0x3F) << 6) |
                       (text[3] & 0x3F));
            }
            else {
                ans.push_back('?');
                i+=1;
            }
        }        
        return ans;
    }
};