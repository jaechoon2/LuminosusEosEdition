// Copyright (c) 2016 Electronic Theatre Controls, Inc., http://www.etcconnect.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef DECAYBLOCK_H
#define DECAYBLOCK_H

#include "core/block_data/InOutBlock.h"
#include "utils.h"

#include <QTimer>


class DecayBlock : public InOutBlock
{
	Q_OBJECT

	Q_PROPERTY(qreal decay READ getDecay WRITE setDecay NOTIFY decayChanged)
	Q_PROPERTY(bool outputIsActive READ getOutputIsActive WRITE setOutputIsActive NOTIFY outputIsActiveChanged)

public:

	static BlockInfo info() {
		static BlockInfo info;
		info.typeName = "Decay";
        info.category << "General" << "Modify Value";
        info.helpText = "Limits the time the value is high. It sets the output back to zero after "
                        "the given time ends.\n\n"
                        "Useful to only send a short pulse when the input is higher than 0 regardless "
                        "of how long the input will stay at that value.\n"
                        "The value can go back to zero before the given time ends. "
                        "To prevent that a Delay Block with Off-Time set can be used.";
        info.qmlFile = "qrc:/qml/Blocks/Logic/DecayBlock.qml";
		info.complete<DecayBlock>();
		return info;
	}

	explicit DecayBlock(MainController* controller, QString uid);

	virtual void getAdditionalState(QJsonObject& state) const override;
	virtual void setAdditionalState(const QJsonObject& state) override;

signals:
	void decayChanged();
	void outputIsActiveChanged();

public slots:
	virtual BlockInfo getBlockInfo() const override { return info(); }

	qreal getDecay() const { return m_decay; }
	void setDecay(qreal value) { m_decay = limit(0, value, 999); emit decayChanged(); }

	bool getOutputIsActive() const { return m_outputIsActive; }
	void setOutputIsActive(bool value) { m_outputIsActive = value; emit outputIsActiveChanged(); }


protected:
	// to be called when the trigger from the raw signal is activated
	// - starts onDelayTimer
	void triggerOn();
	// to be called when the trigger from the raw signal is released
	// - starts offDelayTimer
	void triggerOff();

private slots:
	void onValueChanged();

	void onDecayEnd();

protected:
	double		m_lastValue;

	qreal		m_decay;
	bool		m_outputIsActive;  // true if trigger is activated and not yet released

	QTimer		m_decayTimer;
};

#endif // DECAYBLOCK_H
