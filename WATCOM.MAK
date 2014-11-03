
# MAKE file for WATCOM.MAK

# Revised:  1996-01-18

# Macro definitions

CC=Wcl386
RC=Rc
C_SW=-ms -5s -s -ox -zp4 
RC_SW=-r 


MakePtr.Exe: AppDefs.Obj GetBmp.Obj MakePtr.Obj\
        MakePtr.Res OpenDlg.Obj Support.Obj\
        ViewDlg.Obj ViewWnd.Obj
 WLink @OS2PM_2x.Lnk
 rc makeptr.res MakePtr.Exe

AppDefs.Obj: AppDefs.C
 $(CC) $(C_SW) -fo=$*.Obj AppDefs.C

GetBmp.Obj: GetBmp.C
 $(CC) $(C_SW) -fo=$*.Obj GetBmp.C

MakePtr.Obj: MakePtr.C
 $(CC) $(C_SW) -fo=$*.Obj MakePtr.C

MakePtr.Res: MakePtr.Rc
 $(RC) $(RC_SW) MakePtr.Rc $*.Res

OpenDlg.Obj: OpenDlg.C
 $(CC) $(C_SW) -fo=$*.Obj OpenDlg.C

Support.Obj: Support.C
 $(CC) $(C_SW) -fo=$*.Obj Support.C

ViewDlg.Obj: ViewDlg.C
 $(CC) $(C_SW) -fo=$*.Obj ViewDlg.C

ViewWnd.Obj: ViewWnd.C
 $(CC) $(C_SW) -fo=$*.Obj ViewWnd.C

