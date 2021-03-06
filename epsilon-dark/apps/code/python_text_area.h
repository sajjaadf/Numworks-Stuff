#ifndef CODE_PYTHON_TEXT_AREA_H
#define CODE_PYTHON_TEXT_AREA_H

#include <escher/text_area.h>

namespace Code {

class App;

class PythonTextArea : public TextArea {
public:
  PythonTextArea(Responder * parentResponder, App * pythonDelegate, KDText::FontSize fontSize) :
    TextArea(parentResponder, &m_contentView, fontSize),
    m_contentView(pythonDelegate, fontSize)
  {
  }
  void loadSyntaxHighlighter() { m_contentView.loadSyntaxHighlighter(); }
  void unloadSyntaxHighlighter() { m_contentView.unloadSyntaxHighlighter(); }
protected:
  class ContentView : public TextArea::ContentView {
  public:
    ContentView(App * pythonDelegate, KDText::FontSize fontSize) :
      TextArea::ContentView(fontSize),
      m_pythonDelegate(pythonDelegate)
    {
    }
    void loadSyntaxHighlighter();
    void unloadSyntaxHighlighter();
    void clearRect(KDContext * ctx, KDRect rect) const override;
    void drawLine(KDContext * ctx, int line, const char * text, size_t length, int fromColumn, int toColumn) const override;
    KDRect dirtyRectFromCursorPosition(size_t index, bool lineBreak) const override;
  private:
    App * m_pythonDelegate;
  };
private:
  const ContentView * nonEditableContentView() const override { return &m_contentView; }
  ContentView m_contentView;
};

}

#endif
