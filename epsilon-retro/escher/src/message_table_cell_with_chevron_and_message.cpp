#include <escher/message_table_cell_with_chevron_and_message.h>
#include <escher/palette.h>

MessageTableCellWithChevronAndMessage::MessageTableCellWithChevronAndMessage(KDText::FontSize labelSize, KDText::FontSize contentSize) :
  MessageTableCellWithChevron((I18n::Message)0, labelSize),
  m_subtitleView(contentSize, (I18n::Message)0, 1.0f, 0.5f, Palette::GreyDark)
{
}

View * MessageTableCellWithChevronAndMessage::subAccessoryView() const {
  return (View *)&m_subtitleView;
}

void MessageTableCellWithChevronAndMessage::setHighlighted(bool highlight) {
  MessageTableCellWithChevron::setHighlighted(highlight);
  KDColor backgroundColor = isHighlighted()? Palette::Select : KDColorWhite;
  m_subtitleView.setBackgroundColor(backgroundColor);
}

void MessageTableCellWithChevronAndMessage::setSubtitle(I18n::Message text) {
  m_subtitleView.setMessage(text);
  reloadCell();
  layoutSubviews();
}
