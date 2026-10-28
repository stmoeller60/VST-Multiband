#include "PluginEditor.h"
#include "BinaryData.h"

SteveSledgeCompressorAudioProcessorEditor::SteveSledgeCompressorAudioProcessorEditor (SteveSledgeCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    brandArtwork = juce::ImageCache::getFromMemory (
        BinaryData::maintainer_spectral_compressor_png,
        BinaryData::maintainer_spectral_compressor_pngSize);
    setSize (1250, 900);
    addAndMakeVisible (simpleModeButton);
    simpleModeButton.setClickingTogglesState (true);
    setupSlider (compSlider, compLabel, "COMP  0...10"); setupSlider (attackSlider, attackLabel, "ATTACK  Fast...Slow");
    setupSlider (ratioSlider, ratioLabel, "RATIO"); setupSlider (masterSlider, masterLabel, "MASTER  post limiter");
    setupSlider (inputSlider, inputLabel, "Input"); setupSlider (thresholdSlider, thresholdLabel, "Threshold");
    setupSlider (speedSlider, speedLabel, "Speed"); setupSlider (makeupSlider, makeupLabel, "Makeup");
    setupSlider (ceilingSlider, ceilingLabel, "Ceiling"); setupSlider (xover1Slider, xover1Label, "XOVER 1");
    setupSlider (xover2Slider, xover2Label, "XOVER 2"); setupSlider (xover3Slider, xover3Label, "XOVER 3");
    simpleAttachment = std::make_unique<ButtonAttachment> (processor.apvts, "simple", simpleModeButton);
    compAttachment = std::make_unique<SliderAttachment> (processor.apvts, "comp", compSlider); attackAttachment = std::make_unique<SliderAttachment> (processor.apvts, "attack", attackSlider);
    ratioAttachment = std::make_unique<SliderAttachment> (processor.apvts, "ratio", ratioSlider); masterAttachment = std::make_unique<SliderAttachment> (processor.apvts, "master", masterSlider);
    inputAttachment = std::make_unique<SliderAttachment> (processor.apvts, "input", inputSlider); thresholdAttachment = std::make_unique<SliderAttachment> (processor.apvts, "threshold", thresholdSlider);
    speedAttachment = std::make_unique<SliderAttachment> (processor.apvts, "speed", speedSlider); makeupAttachment = std::make_unique<SliderAttachment> (processor.apvts, "makeup", makeupSlider);
    ceilingAttachment = std::make_unique<SliderAttachment> (processor.apvts, "ceiling", ceilingSlider); xover1Attachment = std::make_unique<SliderAttachment> (processor.apvts, "xover1", xover1Slider);
    xover2Attachment = std::make_unique<SliderAttachment> (processor.apvts, "xover2", xover2Slider); xover3Attachment = std::make_unique<SliderAttachment> (processor.apvts, "xover3", xover3Slider);
    lastSimpleMode = simpleModeButton.getToggleState();
    simpleModeButton.onClick = [this] { const bool s = simpleModeButton.getToggleState(); if (lastSimpleMode && !s) processor.syncAdvancedFromSimple(); lastSimpleMode=s; updateModeVisibility(); resized(); repaint(); };
    updateModeVisibility(); startTimerHz (30);
}

void SteveSledgeCompressorAudioProcessorEditor::setupSlider (juce::Slider& s, juce::Label& l, const juce::String& n)
{ s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag); s.setTextBoxStyle (juce::Slider::TextBoxBelow,false,90,20); addAndMakeVisible(s); l.setText(n,juce::dontSendNotification); l.setJustificationType(juce::Justification::centred); addAndMakeVisible(l); }

void SteveSledgeCompressorAudioProcessorEditor::updateModeVisibility()
{
    const bool s=simpleModeButton.getToggleState(); compSlider.setVisible(s); compLabel.setVisible(s); attackSlider.setVisible(s); attackLabel.setVisible(s);
    ratioSlider.setVisible(true); ratioLabel.setVisible(true); masterSlider.setVisible(true); masterLabel.setVisible(true);
    inputSlider.setVisible(!s); inputLabel.setVisible(!s); thresholdSlider.setVisible(!s); thresholdLabel.setVisible(!s); speedSlider.setVisible(!s); speedLabel.setVisible(!s); makeupSlider.setVisible(!s); makeupLabel.setVisible(!s); ceilingSlider.setVisible(!s); ceilingLabel.setVisible(!s);
    xover1Slider.setVisible(!s); xover1Label.setVisible(!s); xover2Slider.setVisible(!s); xover2Label.setVisible(!s); xover3Slider.setVisible(!s); xover3Label.setVisible(!s);
}

void SteveSledgeCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Branding-only change. Audio DSP, parameter IDs, automation and meter sources
    // are completely unchanged from the last stable main build.
    const auto gold = juce::Colour (0xffe4b86f);
    const auto dim = juce::Colour (0xffaeb4bc);
    const auto panel = juce::Colour (0xff20242b);
    const std::array<juce::Colour, 5> colours {
        juce::Colour (0xff68c7e6), juce::Colour (0xff72d6a7),
        juce::Colour (0xfff2c168), juce::Colour (0xffba91e0),
        juce::Colour (0xffffaa65)
    };

    g.fillAll (juce::Colour (0xff141519));
    g.setColour (juce::Colour (0xff1c1e24));
    g.fillRoundedRectangle (juce::Rectangle<float> (15.0f, 11.0f, 1220.0f, 877.0f), 12.0f);
    g.setColour (juce::Colour (0xff654b33));
    g.drawRoundedRectangle (juce::Rectangle<float> (15.0f, 11.0f, 1220.0f, 877.0f), 12.0f, 1.0f);

    g.setColour (gold);
    g.setFont (juce::FontOptions (24.0f, juce::Font::bold));
    g.drawText ("STEVE SLEDGE  |  SPECTRAL DYNAMICS", 34, 19, 950, 34,
                juce::Justification::centredLeft);
    g.setFont (12.0f);
    g.setColour (dim);
    g.drawText (simpleModeButton.getToggleState()
                    ? "SIMPLE  •  4-band multiband compressor  •  stereo-linked limiter"
                    : "ADVANCED  •  4 compressor bands  •  3 adjustable crossovers  •  stereo-linked 5 ms lookahead limiter",
                35, 54, 1060, 18, juce::Justification::centredLeft);
    g.setColour (juce::Colour (0xff8b6338));
    g.drawLine (35.0f, 79.0f, 1215.0f, 79.0f, 1.0f);

    auto artworkBox = juce::Rectangle<float> (32.0f, 112.0f, 328.0f, 335.0f);
    g.setColour (panel);
    g.fillRoundedRectangle (artworkBox, 9.0f);
    g.setColour (juce::Colour (0xff76512f));
    g.drawRoundedRectangle (artworkBox, 9.0f, 1.0f);
    if (brandArtwork.isValid())
        g.drawImageWithin (brandArtwork, 37, 119, 318, 318, juce::RectanglePlacement::centred);
    else
    {
        g.setColour (gold);
        g.drawText ("RED CROWN  /  MAINTAINER", 45, 220, 304, 40,
                    juce::Justification::centred);
    }
    g.setColour (dim);
    g.setFont (12.0f);
    g.drawText ("RED CROWN SERIES  •  MAINTAINER", 40, 454, 312, 20,
                juce::Justification::centred);

    // Four-second scrolling history at exactly the existing 30 Hz GUI update rate.
    const auto graph = juce::Rectangle<float> (412.0f, 127.0f, 788.0f, 230.0f);
    g.setColour (panel);
    g.fillRoundedRectangle (graph, 7.0f);
    g.setColour (juce::Colour (0xff4b4541));
    g.drawRoundedRectangle (graph, 7.0f, 1.0f);
    g.setColour (gold);
    g.setFont (12.0f);
    g.drawText ("GAIN REDUCTION HISTORY   •   4 SECONDS", 414, 99, 455, 19,
                juce::Justification::centredLeft);
    g.setFont (12.0f);
    g.setColour (outputPeakDb > -0.1f ? juce::Colour (0xffff7575)
                                      : juce::Colour (0xffc2dbe5));
    g.drawText ("Post-Master Peak: " + juce::String (outputPeakDb, 1) + " dBFS",
                935, 99, 265, 19, juce::Justification::centredRight);
    for (int db = 0; db <= 24; db += 6)
    {
        const auto y = graph.getY() + graph.getHeight() * (float) db / 24.0f;
        g.setColour (juce::Colour (0xff49464a));
        g.drawHorizontalLine ((int) y, graph.getX(), graph.getRight());
        g.setColour (dim);
        g.setFont (11.0f);
        g.drawText (db == 0 ? "0 dB" : "-" + juce::String (db),
                    369, (int) y - 8, 37, 17, juce::Justification::centredRight);
    }
    for (int band = 0; band < 5; ++band)
    {
        juce::Path path;
        for (int n = 0; n < historySize; ++n)
        {
            const auto idx = (historyWritePos + n) % historySize;
            const float value = juce::jlimit (0.0f, 24.0f,
                grHistory[(size_t) band][(size_t) idx]);
            const float x = graph.getX() + graph.getWidth() * (float) n / (float) (historySize - 1);
            const float y = graph.getY() + graph.getHeight() * value / 24.0f;
            if (n == 0) path.startNewSubPath (x, y); else path.lineTo (x, y);
        }
        g.setColour (colours[(size_t) band]);
        g.strokePath (path, juce::PathStrokeType (band == 4 ? 2.0f : 1.65f));
    }
    const char* ticks[] = { "-4s", "-3s", "-2s", "-1s", "now" };
    g.setColour (dim);
    g.setFont (10.5f);
    for (int i = 0; i < 5; ++i)
        g.drawText (ticks[i], 396 + i * 197, 360, 38, 17, juce::Justification::centred);

    const char* names[] = { "Band 1", "Band 2", "Band 3", "Band 4", "Limiter" };
    for (int i = 0; i < 5; ++i)
    {
        g.setColour (colours[(size_t) i]);
        g.fillRoundedRectangle ((float) (435 + i * 152), 392.0f, 24.0f, 3.0f, 1.5f);
        g.drawText (names[i], 462 + i * 152, 381, 102, 20, juce::Justification::centredLeft);
    }

    // Keep the five existing instantaneous meters and their numeric labels.
    constexpr int meterTop = 418, meterHeight = 139, meterWidth = 77, gap = 75;
    constexpr int startX = 465;
    for (int i = 0; i < 5; ++i)
    {
        const int x = startX + i * (meterWidth + gap);
        auto area = juce::Rectangle<float> ((float) x, (float) meterTop,
                                            (float) meterWidth, (float) meterHeight);
        g.setColour (juce::Colour (0xff2c3039));
        g.fillRoundedRectangle (area, 5.0f);
        const float db = juce::jlimit (0.0f, 24.0f, meterDb[(size_t) i]);
        const auto fill = area.withY (area.getBottom() - area.getHeight() * db / 24.0f)
                              .withHeight (area.getHeight() * db / 24.0f);
        g.setColour (colours[(size_t) i]);
        g.fillRoundedRectangle (fill, 4.0f);
        g.setColour (juce::Colours::white);
        g.setFont (12.0f);
        g.drawText (names[i], x - 8, meterTop + meterHeight + 5, meterWidth + 16, 16,
                    juce::Justification::centred);
        g.drawText (juce::String (db, 1) + " dB", x - 8, meterTop + meterHeight + 24,
                    meterWidth + 16, 17, juce::Justification::centred);
    }

    g.setColour (juce::Colour (0xff785938));
    g.drawLine (34.0f, 601.0f, 1216.0f, 601.0f, 1.0f);
    g.setColour (dim);
    g.setFont (11.0f);
    g.drawText ("Stereo-linked controls  •  original DSP and parameter automation unchanged",
                25, 868, 1200, 18, juce::Justification::centred);
}

void SteveSledgeCompressorAudioProcessorEditor::resized()
{
    simpleModeButton.setBounds (getWidth() - 127, 23, 104, 26);
    if (simpleModeButton.getToggleState())
    {
        constexpr int y = 650, w = 175, h = 122, gap = 54;
        const int x0 = (getWidth() - (4 * w + 3 * gap)) / 2;
        std::array<juce::Slider*, 4> sliders { &compSlider, &ratioSlider, &attackSlider, &masterSlider };
        std::array<juce::Label*, 4> labels { &compLabel, &ratioLabel, &attackLabel, &masterLabel };
        for (int i = 0; i < 4; ++i)
        {
            const int x = x0 + i * (w + gap);
            labels[(size_t) i]->setBounds (x, y - 22, w, 19);
            sliders[(size_t) i]->setBounds (x, y, w, h);
        }
    }
    else
    {
        constexpr int w = 167, h = 104, gap = 54, row1 = 630, row2 = 760;
        const int x0 = (getWidth() - (5 * w + 4 * gap)) / 2;
        std::array<juce::Slider*, 5> row1Sliders { &inputSlider, &thresholdSlider, &ratioSlider, &speedSlider, &makeupSlider };
        std::array<juce::Label*, 5> row1Labels { &inputLabel, &thresholdLabel, &ratioLabel, &speedLabel, &makeupLabel };
        std::array<juce::Slider*, 5> row2Sliders { &xover1Slider, &xover2Slider, &xover3Slider, &ceilingSlider, &masterSlider };
        std::array<juce::Label*, 5> row2Labels { &xover1Label, &xover2Label, &xover3Label, &ceilingLabel, &masterLabel };
        for (int i = 0; i < 5; ++i)
        {
            const int x = x0 + i * (w + gap);
            row1Labels[(size_t) i]->setBounds (x, row1 - 23, w, 20);
            row1Sliders[(size_t) i]->setBounds (x, row1, w, h);
            row2Labels[(size_t) i]->setBounds (x, row2 - 23, w, 20);
            row2Sliders[(size_t) i]->setBounds (x, row2, w, h);
        }
    }
}

void SteveSledgeCompressorAudioProcessorEditor::timerCallback()
{
    for(int i=0;i<4;++i) meterDb[(size_t)i]=processor.getBandMeterDb(i); meterDb[4]=processor.getLimiterMeterDb(); outputPeakDb=processor.getOutputPeakDb();
    for(int i=0;i<5;++i) grHistory[(size_t)i][(size_t)historyWritePos]=meterDb[(size_t)i]; historyWritePos=(historyWritePos+1)%historySize;
    const bool s=simpleModeButton.getToggleState(); if(s!=lastSimpleMode){if(lastSimpleMode&&!s)processor.syncAdvancedFromSimple();lastSimpleMode=s;updateModeVisibility();resized();} repaint();
}
