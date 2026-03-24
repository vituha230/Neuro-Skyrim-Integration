Scriptname myscScript extends ObjectReference  

Int RunOnce
Quest Property myscPath Auto
Actor Property PlayerRef Auto

  Event OnTriggerEnter(ObjectReference akActionRef)
    If akActionRef == PlayerRef

   myscPath.SetStage(10)

   

   EndIf

myscPath.Start()
myscPath.SetStage(0)
myscPath.SetStage(10)

 EndEvent