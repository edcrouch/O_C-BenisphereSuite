// Copyright (c) 2018, Jason Justian
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP  0xFC

class ClockSetup : public HemisphereApplet {
public:

    const char* applet_name() {
        return "ClockSet";
    }

    void Start() { }

    // The ClockSetup controller handles MIDI Clock and Transport Start/Stop
    void Controller() {
        if (start_q){
            start_q = 0;
            usbMIDI.sendRealTime(MIDI_START);
        }
        if (stop_q){
            stop_q = 0;
            usbMIDI.sendRealTime(MIDI_STOP);
        }
        if (clock_m->IsRunning() && clock_m->MIDITock()) usbMIDI.sendRealTime(MIDI_CLOCK);
    }

    void View() {
        DrawInterface();
    }

    void OnButtonPress() {
        isEditing = !isEditing;
    }

    void OnEncoderMove(int direction) {
        if (!isEditing) {
            cursor = constrain(cursor + direction, 0, 3);
            ResetCursor();
        } else {
            switch (cursor) {
            case 0: // Source
                if (direction > 0) // right turn toggles Forwarding
                    clock_m->ToggleForwarding();
                else if (clock_m->IsRunning()) // left turn toggles clock
                {
                    stop_q = 1;
                    clock_m->Stop();
                }
                else {
                    start_q = 1;
                    clock_m->Start();
                }
                break;

            case 1: // Set tempo
                clock_m->SetTempoBPM(clock_m->GetTempo() + direction);
                break;

            case 2: // Set multiplier
            case 3:
                clock_m->SetMultiply(clock_m->GetMultiply(cursor-2) + direction, cursor-2);
                break;
            }
        }
    }

    uint64_t OnDataRequest() {
        uint64_t data = 0;
        Pack(data, PackLocation { 0, 1 }, clock_m->IsRunning() || clock_m->IsPaused());
        Pack(data, PackLocation { 1, 9 }, clock_m->GetTempo());
        Pack(data, PackLocation { 10, 5 }, clock_m->GetMultiply());
        Pack(data, PackLocation { 15, 1 }, clock_m->IsForwarded());
        return data;
    }

    void OnDataReceive(uint64_t data) {
        if (Unpack(data, PackLocation { 0, 1 })) {
            clock_m->Start(1); // start paused
        } else {
            clock_m->Stop();
        }
        clock_m->SetTempoBPM(Unpack(data, PackLocation { 1, 9 }));
        clock_m->SetMultiply(Unpack(data, PackLocation { 10, 5 }));
        clock_m->SetForwarding(Unpack(data, PackLocation { 15, 1 }));
    }

protected:
    void SetHelp() {
        //                               "------------------" <-- Size Guide
        help[HEMISPHERE_HELP_DIGITALS] = "";
        help[HEMISPHERE_HELP_CVS]      = "";
        help[HEMISPHERE_HELP_OUTS]     = "";
        help[HEMISPHERE_HELP_ENCODER]  = "";
        //                               "------------------" <-- Size Guide
    }

private:
    char cursor; // 0=Source, 1=Tempo, 2=Multiply
    bool isEditing = false;
    bool start_q;
    bool stop_q;
    ClockManager *clock_m = clock_m->get();

    void DrawInterface() {
        // Header: This is sort of a faux applet, so its header
        // needs to extend across the screen
        graphics.setPrintPos(1, 2);
        graphics.print("Clock Setup");
        //gfxLine(0, 10, 62, 10);
        //gfxLine(0, 12, 62, 12);
        graphics.drawLine(0, 10, 127, 10);
        graphics.drawLine(0, 12, 127, 12);

        // Clock Source
        gfxIcon(1, 15, CLOCK_ICON);
        if (clock_m->IsRunning()) {
            gfxIcon(10, 15, PLAY_ICON);
        } else if (clock_m->IsPaused()) {
            gfxIcon(10, 15, PAUSE_ICON);
        } else {
            gfxIcon(10, 15, STOP_ICON);
        }
        gfxPrint(20, 15, "<Clk/Fwd>");
        if (clock_m->IsForwarded())
            gfxIcon(76, 15, LINK_ICON);

        // Tempo
        gfxIcon(1, 25, NOTE4_ICON);
        gfxPrint(9, 25, "= ");
        gfxPrint(pad(100, clock_m->GetTempo()), clock_m->GetTempo());
        gfxPrint(" BPM");

        // Multiply
        gfxPrint(1, 35, "x");
        gfxPrint(clock_m->GetMultiply(0));

        // secondary multiplier when forwarding internal clock
        gfxPrint(33, 35, "x");
        gfxPrint(clock_m->GetMultiply(1));

        switch (cursor) {
        case 0:
            gfxCursor(20, 23, 54);
            if (isEditing) gfxInvert(20, 14, 54, 9);
            break;
        case 1:
            gfxCursor(23, 33, 18);
            if (isEditing) gfxInvert(23, 24, 18, 9);
            break;
        case 2:
            gfxCursor(8, 43, 12);
            if (isEditing) gfxInvert(8, 34, 12, 9);
            break;
        case 3:
            gfxCursor(40, 43, 12);
            if (isEditing) gfxInvert(40, 34, 12, 9);
            break;
        }
    }
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to ClockSetup,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
ClockSetup ClockSetup_instance[1];

void ClockSetup_Start(bool hemisphere) {ClockSetup_instance[hemisphere].BaseStart(hemisphere);}
void ClockSetup_Controller(bool hemisphere, bool forwarding) {ClockSetup_instance[hemisphere].BaseController(forwarding);}
void ClockSetup_View(bool hemisphere) {ClockSetup_instance[hemisphere].BaseView();}
void ClockSetup_OnButtonPress(bool hemisphere) {ClockSetup_instance[hemisphere].OnButtonPress();}
void ClockSetup_OnEncoderMove(bool hemisphere, int direction) {ClockSetup_instance[hemisphere].OnEncoderMove(direction);}
void ClockSetup_ToggleHelpScreen(bool hemisphere) {ClockSetup_instance[hemisphere].HelpScreen();}
uint64_t ClockSetup_OnDataRequest(bool hemisphere) {return ClockSetup_instance[hemisphere].OnDataRequest();}
void ClockSetup_OnDataReceive(bool hemisphere, uint64_t data) {ClockSetup_instance[hemisphere].OnDataReceive(data);}
