// This 'ere software is hacky as fuck. Use at your peril.
// I'd appreciate a mention if you do. But if you don't,
// I'm not going to lose sleep over it.
// Ben H.D. Jones in the year of 2018

#pragma once

#include "glfreetype/TextRenderer.hpp"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#include <cctype>
#include <sstream>

namespace glconsole {
    class GLConsole
    {
      public:
        GLConsole(int const width, 
                  int const height,
                  int const x,
                  int const y,
                  int const fontSize,
                  std::string const & fontPath)
          : m_width(width)
          , m_height(height)
          , m_x(x)
          , m_y(y)
          , m_buffer()
          , m_linesCommitted()
          , m_prompt(">> |")
        {
            m_font.init(fontPath.c_str(), fontSize);

            // Begin what the user will see!
            m_buffer << m_prompt;
        }

        void display()
        {
            glPushMatrix();
            glLoadIdentity();
            std::stringstream ss;
            glfreetype::print(m_font, m_x, m_y, m_buffer.str());
            glPopMatrix();
        }

        void keyHandler(int const key, 
                        int const scancode, 
                        int const action, 
                        int const mods) 
        {
            if (action == GLFW_PRESS) {
                if (key != GLFW_KEY_BACKSPACE) {

                    // Figure out if char should be upper or lowercase.
                    // By default, GLFW seems to yield uppercase chars.
                    auto upperCase = false;
                    if(mods == GLFW_MOD_SHIFT) {
                        upperCase = true;
                    }

                    // Don't print out shift key chars
                    if(key != GLFW_KEY_LEFT_SHIFT &&
                       key != GLFW_KEY_RIGHT_SHIFT) {

                        // Before appending the new char, need
                        // to delete the '|' char.
                        auto before = m_buffer.str();
                        auto after = before.substr(0, before.size() - 1);
                        m_buffer.str("");
                        m_buffer << after;

                        if(key != GLFW_KEY_ENTER) {
                            // Now append the char
                            auto casedChar = char(key);
                            if(!upperCase) {
                                casedChar = std::tolower(casedChar);
                            }

                            // ...finishing with the '|' char.
                            m_buffer << casedChar << '|';
                        } else {
                            // New line
                            m_buffer << "\n"  << m_prompt;

                            // Store buffer so far (in aid of
                            // a bug-free backspace process).
                            m_linesCommitted = m_buffer.str();
                        }
                    }
                } else {
                    handleBackspace();
                }
            }
        }

      private:

        /// Width of text area
        int m_width;

        /// Height of text area
        int m_height;

        /// X and Y position of where text is rendered
        /// relative to width and height
        int m_x;
        int m_y;

        /// Stores information related to the font
        glfreetype::font_data m_font;

        /// Stores text typed so far
        std::stringstream m_buffer;

        /// Stores previously entered line data.
        std::string m_linesCommitted;

        /// The actual prompt, hardcoded to ">>"
        std::string m_prompt;

        /// Does what it sayd.
        void handleBackspace()
        {
            auto before = m_buffer.str();

            // Only permit stuff after ">> ".
            if(!before.empty() && before.length() > 4) {
                auto after = before.substr(0, before.length() - 2);

                // Make sure that we don't delete newline chars
                // and ">> " in the case of a newline.
                if(m_linesCommitted.empty() ||
                   after.length() >= m_linesCommitted.length() - 1) {
                    m_buffer.str("");
                    m_buffer << after << "|";
                }
            }
        }
    };
}