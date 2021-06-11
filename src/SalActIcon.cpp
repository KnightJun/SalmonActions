#include "SalActIcon.h"

#define defIcon(NAME)  QIcon SalActIcon::NAME() \
{\
    return QIcon(":/Icons/" #NAME ".png"); \
}

defIcon(Arrow);
defIcon(Copy);
defIcon(CopyStamp);
defIcon(Eraser);
defIcon(OrderStamp);
defIcon(PasteImage);
defIcon(Pencil);
defIcon(Polyline);
defIcon(Quit);
defIcon(Record);
defIcon(StopRecord);
defIcon(RectCircle);
defIcon(Text);
defIcon(Undo);
defIcon(Redo);
defIcon(Mosaic);
defIcon(Info);
defIcon(LongShot);
defIcon(BeginLongShot);
defIcon(ReAdjust);
defIcon(Save);
defIcon(Move);
defIcon(MoveHorizontal);
defIcon(MoveVertical);
defIcon(Close);
defIcon(Keyboard);
defIcon(Setting);
defIcon(Location);
defIcon(Pause);
defIcon(Play);
defIcon(Gif);
defIcon(Png);
defIcon(Webp);
defIcon(Return);
defIcon(Dga);