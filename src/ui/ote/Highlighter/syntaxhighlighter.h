/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>
    Modified 2018 Julian Bansen <https://github.com/JuBan1>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OTE_QSYNTAXHIGHLIGHTER_H
#define OTE_QSYNTAXHIGHLIGHTER_H

#include "abstracthighlighter.h"

#include <QObject>
#include <QString>
#include <QTextBlock>
#include <QTextDocument>
#include <QSyntaxHighlighter>

namespace ote {

class SyntaxHighlighterPrivate;

/** A QSyntaxHighlighter implementation for use with QTextDocument.
 *  This supports partial re-highlighting during editing and
 *  tracks syntax-based code folding regions.
 *
 *  @since 5.28
 */
class SyntaxHighlighter : public QSyntaxHighlighter, public AbstractHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr);
    explicit SyntaxHighlighter(QTextDocument *document);
    ~SyntaxHighlighter() override;


    void setTheme(const Theme &theme) override;

    void setDefinition(const Definition &def) override;

    /** Returns whether there is a folding region beginning at @p startBlock.
     *  This only considers syntax-based folding regions,
     *  not indention-based ones as e.g. found in Python.
     *
     *  @see findFoldingRegionEnd
     */
    bool startsFoldingRegion(const QTextBlock &startBlock) const;

    /** Finds the end of the folding region starting at @p startBlock.
     *  If multiple folding regions begin at @p startBlock, the end of
     *  the last/innermost one is returned.
     *  This returns an invalid block if no folding region end is found,
     *  which typically indicates an unterminated region and thus folding
     *  until the document end.
     *  This method performs a sequential search starting at @p startBlock
     *  for the matching folding region end, which is a potentially expensive
     *  operation.
     *
     *  @see startsFoldingRegion
     */
    QTextBlock findFoldingRegionEnd(const QTextBlock &startBlock) const;

    bool isPositionInComment(int absPos, int len=0) const;

    /** Rehighlights the entire document. Rather than blocking the main thread
     *  it will only process one block with each program tick. As a result,
     *  highlighting large documents won't freeze the program but might take
     *  a while to fully highlight.
     */
    void startRehighlighting();

signals:
    /**
     * Is emitted when the highlighter is finished with a new block. That means
     * text or highlighting state of this block may have changed.
     */
    void blockChanged(const QTextBlock& block);

protected:
    void highlightBlock(const QString & text) override;
    void applyFormat(int offset, int length, const Format &format) override;
    void applyFolding(int offset, int length, FoldingRegion region) override;

private:
    Q_DECLARE_PRIVATE_D(AbstractHighlighter::d_ptr, SyntaxHighlighter)
};
}

#endif // OTE_QSYNTAXHIGHLIGHTER_H
