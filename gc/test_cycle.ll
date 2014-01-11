; ModuleID = 'test_cycle.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._list = type { [2 x i32], %struct._list* }

define %struct._list* @create() nounwind uwtable {
  %l = alloca %struct._list*, align 8
  %1 = call noalias i8* @malloc(i64 16) nounwind
  %2 = bitcast i8* %1 to %struct._list*
  store %struct._list* %2, %struct._list** %l, align 8
  %3 = load %struct._list** %l, align 8
  %4 = getelementptr inbounds %struct._list* %3, i32 0, i32 0
  %5 = getelementptr inbounds [2 x i32]* %4, i32 0, i64 0
  store i32 1, i32* %5, align 4
  %6 = load %struct._list** %l, align 8
  %7 = getelementptr inbounds %struct._list* %6, i32 0, i32 0
  %8 = getelementptr inbounds [2 x i32]* %7, i32 0, i64 1
  store i32 8, i32* %8, align 4
  %9 = load %struct._list** %l, align 8
  %10 = getelementptr inbounds %struct._list* %9, i32 0, i32 1
  store %struct._list* null, %struct._list** %10, align 8
  %11 = load %struct._list** %l, align 8
  ret %struct._list* %11
}

declare noalias i8* @malloc(i64) nounwind

declare void @llvm.gcroot(i8** %ptrloc, i8* %metadata)

define i32 @main() nounwind uwtable gc "jblab-gc" {
  %1 = alloca i32, align 4
  %f = alloca %struct._list*, align 8
  %c = alloca %struct._list*, align 8
  %tmp_f = bitcast %struct._list** %f to i8**   
  %tmp_c = bitcast %struct._list** %c to i8**   
  call void @llvm.gcroot(i8** %tmp_f, i8* null)
  call void @llvm.gcroot(i8** %tmp_c, i8* null)
  store i32 0, i32* %1
  %2 = call %struct._list* @create()
  store %struct._list* %2, %struct._list** %f, align 8
  %3 = call %struct._list* @create()
  %4 = load %struct._list** %f, align 8
  %5 = getelementptr inbounds %struct._list* %4, i32 0, i32 1
  store %struct._list* %3, %struct._list** %5, align 8
  %6 = call %struct._list* @create()
  %7 = load %struct._list** %f, align 8
  %8 = getelementptr inbounds %struct._list* %7, i32 0, i32 1
  %9 = load %struct._list** %8, align 8
  %10 = getelementptr inbounds %struct._list* %9, i32 0, i32 1
  store %struct._list* %6, %struct._list** %10, align 8
  %11 = call %struct._list* @create()
  %12 = call %struct._list* @create()
  %13 = call %struct._list* @create()
  store %struct._list* %13, %struct._list** %c, align 8
  %14 = call %struct._list* @create()
  %15 = load %struct._list** %c, align 8
  %16 = getelementptr inbounds %struct._list* %15, i32 0, i32 1
  store %struct._list* %14, %struct._list** %16, align 8
  %17 = call %struct._list* @create()
  %18 = load %struct._list** %c, align 8
  %19 = getelementptr inbounds %struct._list* %18, i32 0, i32 1
  %20 = load %struct._list** %19, align 8
  %21 = getelementptr inbounds %struct._list* %20, i32 0, i32 1
  store %struct._list* %17, %struct._list** %21, align 8
  %22 = call %struct._list* @create()
  %23 = load %struct._list** %c, align 8
  %24 = getelementptr inbounds %struct._list* %23, i32 0, i32 1
  %25 = load %struct._list** %24, align 8
  %26 = getelementptr inbounds %struct._list* %25, i32 0, i32 1
  %27 = load %struct._list** %26, align 8
  %28 = getelementptr inbounds %struct._list* %27, i32 0, i32 1
  store %struct._list* %22, %struct._list** %28, align 8
  %29 = load %struct._list** %c, align 8
  %30 = getelementptr inbounds %struct._list* %29, i32 0, i32 1
  %31 = load %struct._list** %30, align 8
  %32 = load %struct._list** %c, align 8
  %33 = getelementptr inbounds %struct._list* %32, i32 0, i32 1
  %34 = load %struct._list** %33, align 8
  %35 = getelementptr inbounds %struct._list* %34, i32 0, i32 1
  %36 = load %struct._list** %35, align 8
  %37 = getelementptr inbounds %struct._list* %36, i32 0, i32 1
  %38 = load %struct._list** %37, align 8
  %39 = getelementptr inbounds %struct._list* %38, i32 0, i32 1
  store %struct._list* %31, %struct._list** %39, align 8
  call void (...)* @gc()
  store %struct._list* null, %struct._list** %c, align 8
  call void (...)* @gc()
  ret i32 0
}

declare void @gc(...)
