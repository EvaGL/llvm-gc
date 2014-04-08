; ModuleID = 'autocall_test.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.data = type { i32, [100 x i32] }

; Function Attrs: nounwind uwtable
define %struct.data* @create_data(i32 %iteration) #0 {
entry:
  %iteration.addr = alloca i32, align 4
  %instance = alloca %struct.data*, align 8
  store i32 %iteration, i32* %iteration.addr, align 4
  %call = call i8* @malloc(i64 404)
  %0 = bitcast i8* %call to %struct.data*
  store %struct.data* %0, %struct.data** %instance, align 8
  %1 = load %struct.data** %instance, align 8
  %meta = getelementptr inbounds %struct.data* %1, i32 0, i32 0
  store i32 0, i32* %meta, align 4
  %2 = load i32* %iteration.addr, align 4
  %3 = load %struct.data** %instance, align 8
  %value = getelementptr inbounds %struct.data* %3, i32 0, i32 1
  %arrayidx = getelementptr inbounds [100 x i32]* %value, i32 0, i64 0
  store i32 %2, i32* %arrayidx, align 4
  %4 = load %struct.data** %instance, align 8
  ret %struct.data* %4
}

declare i8* @malloc(i64) #1
declare void @llvm.gcroot(i8**, i8*)

; Function Attrs: nounwind uwtable
define i32 @main() #0 gc "jblab-gc" {
entry:
  %retval = alloca i32, align 4
  %root = alloca %struct.data*, align 8
  %temp = bitcast %struct.data** %root to i8**
  call void @llvm.gcroot(i8** %temp, i8* null)
  %i = alloca i32, align 4
  store i32 0, i32* %retval
  store %struct.data* null, %struct.data** %root, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32* %i, align 4
  %cmp = icmp slt i32 %0, 10000
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32* %i, align 4
  %call = call %struct.data* @create_data(i32 %1)
  store %struct.data* %call, %struct.data** %root, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %2 = load i32* %i, align 4
  %inc = add nsw i32 %2, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 196081)"}
