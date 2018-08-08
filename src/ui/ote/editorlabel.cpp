#include "editorlabel.h"

#include "textedit.h"

#include <QTextDocument>

namespace ote {

void EditorLabel::draw(QPainter& painter, const QPointF& offset)
{
    painter.save();
    painter.drawPixmap(getDisplayRect().translated(offset).topLeft(), m_pixmap);
    /*if(m_renderDebugInfo) {
        painter.setPen(Qt::red);
        painter.drawRect( getDisplayRect().translated(offset) );
    }*/
    painter.restore();
}

bool EditorLabel::updateDisplayRect(qreal rightBorder)
{
    TextEdit* te = dynamic_cast<TextEdit*>(parent());

    const QTextBlock block = te->document()->findBlock(m_absPos);
    const QTextLayout* layout = block.layout();

    if (!layout) {
        m_displayRect = QRectF();
        return true;
    }

    const auto layoutBoundingRect = layout->boundingRect();
    const QTextLine l = layout->lineForTextPosition(m_absPos - block.position());

    if (!l.isValid()) {
        m_displayRect = QRectF();
        return true;
    }

    auto rectStart = m_anchor == AnchorBelowLine ? layoutBoundingRect.bottomLeft() : layoutBoundingRect.topLeft();
    if (m_anchor == AnchorEndOfLine) {
        rectStart.rx() = l.naturalTextRect().right();
    } else {
        // TODO: Perhaps cache the fontmetrics object?
        QFont f = block.document()->defaultFont();
        QFontMetrics fm(f);
        const auto begin = l.textStart();
        const auto length = m_absPos - block.position();
        auto adv = fm.horizontalAdvance(block.text().mid(begin, length));
        rectStart.rx() += adv; // Set left border
    }

    auto rectEnd = layoutBoundingRect.bottomLeft();
    if (rightBorder < 0) // Set right border
        rectEnd.rx() = layoutBoundingRect.right();
    else
        rectEnd.rx() = rightBorder;

    auto nb = block.next();

    int numBlocks = MAX_BLOCK_COUNT;
    int numLines = m_heightInLines > 0 ? m_heightInLines : 99;

    if (m_anchor != AnchorBelowLine) {
        numBlocks--;
        numLines -= block.lineCount();
    }

    while (nb.isValid() && nb.isVisible() && numBlocks > 0 && numLines > 0) {
        auto lay = nb.layout();

        if (!m_overlap && lay->lineCount() > 0) {
            auto lineWidth = lay->lineAt(0).naturalTextWidth();
            if (rectStart.x() < lineWidth)
                break;
            numLines--;
        }

        numBlocks--;

        rectEnd.ry() += nb.layout()->boundingRect().height();
        nb = nb.next();
    }

    const auto newDisplayRect = QRectF(rectStart, rectEnd);
    const bool rectChanged = m_displayRect != newDisplayRect;
    m_displayRect = newDisplayRect;

    /*if (m_renderDebugInfo) {
        qDebug() << m_displayRect << rectChanged;
    }*/

    return rectChanged || m_changed; // TODO: m_changed here needed?
}

EditorLabel::EditorLabel(TextEdit* te, int pos)
    : QObject(te)
    , m_absPos(pos)
{
}

QTextBlock EditorLabel::getTextBlock() const
{
    return getTextEdit()->document()->findBlock(m_absPos);
}

TextEdit* EditorLabel::getTextEdit() const
{
    return dynamic_cast<TextEdit*>(parent());
}

} // namespace ote
