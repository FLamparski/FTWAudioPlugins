declare options "[midi:on][nvoices:8]";

import("stdfaust.lib");

// MIDI hookup: freq is based on note played, gain is velocity, gate is whether the note is on
freq = hslider("freq[hidden:1]",200,50,1000,0.01);
gain = hslider("gain[hidden:1]",0.5,0,1,0.01);
gate = button("gate[hidden:1]");

q = hslider("q", 20, 2, 40, 0.1);
qcomp = 0.5 - 0.025 * q; // Decrease levels at high Q settings

resonbp2x(freq, q) = _ : fi.resonbp(freq, q, 0.5) : fi.resonbp(freq, q, 0.5) : _;

scaledadsr(id, scale) = en.adsr(
        vslider("attack %id", 0.01, 0.001, 4, 0.001),
        vslider("decay %id", 0.3, 0.001, 4, 0.001),
        vslider("sustain %id", 0.6, 0.0, 1.0, 0.001),
        vslider("release %id", 0.5, 0.001, 4, 0.001),
        gate
    ) * scale : _;

band(id, freq, q, basegain) = _ : resonbp2x(freq, q) * adsr : _
with {
    adsr = scaledadsr(id, basegain + qcomp);
};

tonalizer(freq, q) = _ <:
    band(0, freq / 2, q, vslider("partial gain 0", 0.5, 0.0, 1.0, 0.01)),
    band(1, freq, q, vslider("partial gain 1", 1, 0.0, 1.0, 0.01)),
    band(2, freq * 2, q, vslider("partial gain 2", 0.25, 0.0, 1.0, 0.01)),
    band(4, freq * 4, q, vslider("partial gain 4", 0.25, 0.0, 1.0, 0.01)),
    band(6, freq * 6, q, vslider("partial gain 6", 0.25, 0.0, 1.0, 0.01)),
    band(8, freq * 8, q, vslider("partial gain 8", 0.25, 0.0, 1.0, 0.01)),
    band(10, freq * 10, q, vslider("partial gain 10", 0.25, 0.0, 1.0, 0.01)),
    band(12, freq * 12, q, vslider("partial gain 12", 0.25, 0.0, 1.0, 0.01)),
    band(14, freq * 14, q, vslider("partial gain 14", 0.1, 0.0, 1.0, 0.01)),
    band(16, freq * 16, q, vslider("partial gain 16", 0.1, 0.0, 1.0, 0.01))
    :> /(12);

process = no.noise : tonalizer(freq, q) * gain * hslider("volume", 0.5, 0, 1, 0.01) <: _, _;