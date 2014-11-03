
# MAKE file for Borland.MAK

# Revised:  1996-01-18

# Macro definitions

CC=BCC
RC=Rc
C_SW=-Ox -v- 
RC_SW=-r 


MakePtr.Exe: AppDefs GetBmp MakePtr\
        MakePtr.Res OpenDlg Support\
        ViewDlg ViewWnd
 TLink @OS2PM_2x.Lnk
 rc makeptr.res MakePtr.Exe

AppDefs: AppDefs.C
 $(CC) $(C_SW) -o$*.Obj AppDefs.C

GetBmp: GetBmp.C
 $(CC) $(C_SW) -o$*.Obj GetBmp.C

MakePtr: MakePtr.C
 $(CC) $(C_SW) -o$*.Obj MakePtr.C

MakePtr.Res: MakePtr.Rc
 $(RC) $(RC_SW) MakePtr.Rc $*.Res

OpenDlg: OpenDlg.C
 $(CC) $(C_SW) -o$*.Obj OpenDlg.C

Support: Support.C
 $(CC) $(C_SW) -o$*.Obj Support.C

ViewDlg: ViewDlg.C
 $(CC) $(C_SW) -o$*.Obj ViewDlg.C

ViewWnd: ViewWnd.C
 $(CC) $(C_SW) -o$*.Obj ViewWnd.C

