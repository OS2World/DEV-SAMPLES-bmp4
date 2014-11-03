
# MAKE file for IBM.MAK

# Revised:  1996-01-18

# Macro definitions

CC=Icc
RC=Rc
SC=SC
C_SW=-G5 -O+ -c
RC_SW=-r


MakePtr.Exe: AppDefs GetBmp MakePtr\
        MakePtr.Res OpenDlg Support\
        ViewDlg ViewWnd
 ILink /FREE /ALIGN:4 /EXEPACK /BASE:0x10000 @IBM.Lnk
 rc makeptr.res MakePtr.Exe

AppDefs: AppDefs.C
 $(CC) $(C_SW) -Fo$*.Obj AppDefs.C

GetBmp: GetBmp.C
 $(CC) $(C_SW) -Fo$*.Obj GetBmp.C

MakePtr: MakePtr.C
 $(CC) $(C_SW) -Fo$*.Obj MakePtr.C

MakePtr.Res: MakePtr.Rc
 $(RC) $(RC_SW) MakePtr.Rc $*.Res

OpenDlg: OpenDlg.C
 $(CC) $(C_SW) -Fo$*.Obj OpenDlg.C

Support: Support.C
 $(CC) $(C_SW) -Fo$*.Obj Support.C

ViewDlg: ViewDlg.C
 $(CC) $(C_SW) -Fo$*.Obj ViewDlg.C

ViewWnd: ViewWnd.C
 $(CC) $(C_SW) -Fo$*.Obj ViewWnd.C

