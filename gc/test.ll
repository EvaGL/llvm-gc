; ModuleID = 'test.c'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._tree = type { [3 x i32], i32, %struct._tree*, %struct._tree* }

@.str = private unnamed_addr constant [2 x i8] c"_\00", align 1
@.str1 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@.str2 = private unnamed_addr constant [3 x i8] c") \00", align 1
@.str3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define void @show(%struct._tree* %this) nounwind uwtable {
  %1 = alloca %struct._tree*, align 8
  store %struct._tree* %this, %struct._tree** %1, align 8
  %2 = load %struct._tree** %1, align 8
  %3 = icmp ne %struct._tree* %2, null
  br i1 %3, label %6, label %4

; <label>:4                                       ; preds = %0
  %5 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8]* @.str, i32 0, i32 0))
  br label %21

; <label>:6                                       ; preds = %0
  %7 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str1, i32 0, i32 0))
  %8 = load %struct._tree** %1, align 8
  %9 = getelementptr inbounds %struct._tree* %8, i32 0, i32 2
  %10 = load %struct._tree** %9, align 8
  call void @show(%struct._tree* %10)
  %11 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str2, i32 0, i32 0))
  %12 = load %struct._tree** %1, align 8
  %13 = getelementptr inbounds %struct._tree* %12, i32 0, i32 1
  %14 = load i32* %13, align 4
  %15 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str3, i32 0, i32 0), i32 %14)
  %16 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str1, i32 0, i32 0))
  %17 = load %struct._tree** %1, align 8
  %18 = getelementptr inbounds %struct._tree* %17, i32 0, i32 3
  %19 = load %struct._tree** %18, align 8
  call void @show(%struct._tree* %19)
  %20 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str2, i32 0, i32 0))
  br label %21

; <label>:21                                      ; preds = %6, %4
  ret void
}

declare i32 @printf(i8*, ...)

define %struct._tree* @tree_init(i32 %val, %struct._tree* %left_new, %struct._tree* %right_new) nounwind uwtable {
  %1 = alloca i32, align 4
  %2 = alloca %struct._tree*, align 8
  %3 = alloca %struct._tree*, align 8
  %new_tree = alloca %struct._tree*, align 8
  store i32 %val, i32* %1, align 4
  store %struct._tree* %left_new, %struct._tree** %2, align 8
  store %struct._tree* %right_new, %struct._tree** %3, align 8
  %4 = call i8* @malloc(i64 32)
  %5 = bitcast i8* %4 to %struct._tree*
  store %struct._tree* %5, %struct._tree** %new_tree, align 8
  %6 = load %struct._tree** %new_tree, align 8
  %7 = getelementptr inbounds %struct._tree* %6, i32 0, i32 0
  %8 = getelementptr inbounds [3 x i32]* %7, i32 0, i64 0
  store i32 2, i32* %8, align 4
  %9 = load %struct._tree** %new_tree, align 8
  %10 = getelementptr inbounds %struct._tree* %9, i32 0, i32 0
  %11 = getelementptr inbounds [3 x i32]* %10, i32 0, i64 1
  store i32 16, i32* %11, align 4
  %12 = load %struct._tree** %new_tree, align 8
  %13 = getelementptr inbounds %struct._tree* %12, i32 0, i32 0
  %14 = getelementptr inbounds [3 x i32]* %13, i32 0, i64 1
  %15 = load i32* %14, align 4
  %16 = sext i32 %15 to i64
  %17 = add i64 %16, 8
  %18 = trunc i64 %17 to i32
  %19 = load %struct._tree** %new_tree, align 8
  %20 = getelementptr inbounds %struct._tree* %19, i32 0, i32 0
  %21 = getelementptr inbounds [3 x i32]* %20, i32 0, i64 2
  store i32 %18, i32* %21, align 4
  %22 = load i32* %1, align 4
  %23 = load %struct._tree** %new_tree, align 8
  %24 = getelementptr inbounds %struct._tree* %23, i32 0, i32 1
  store i32 %22, i32* %24, align 4
  %25 = load %struct._tree** %2, align 8
  %26 = load %struct._tree** %new_tree, align 8
  %27 = getelementptr inbounds %struct._tree* %26, i32 0, i32 2
  store %struct._tree* %25, %struct._tree** %27, align 8
  %28 = load %struct._tree** %3, align 8
  %29 = load %struct._tree** %new_tree, align 8
  %30 = getelementptr inbounds %struct._tree* %29, i32 0, i32 3
  store %struct._tree* %28, %struct._tree** %30, align 8
  %31 = load %struct._tree** %new_tree, align 8
  ret %struct._tree* %31
}

declare i8* @malloc(i64)

define %struct._tree* @generate_tree_rec(i32 %height, i32 %number) nounwind uwtable {
  %1 = alloca %struct._tree*, align 8
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %left = alloca %struct._tree*, align 8
  %right = alloca %struct._tree*, align 8
  %node = alloca %struct._tree*, align 8
  store i32 %height, i32* %2, align 4
  store i32 %number, i32* %3, align 4
  %4 = load i32* %2, align 4
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  store %struct._tree* null, %struct._tree** %1
  br label %30

; <label>:7                                       ; preds = %0
  %8 = load i32* %2, align 4
  %9 = sub nsw i32 %8, 1
  %10 = load i32* %3, align 4
  %11 = mul nsw i32 %10, 2
  %12 = call %struct._tree* @generate_tree_rec(i32 %9, i32 %11)
  store %struct._tree* %12, %struct._tree** %left, align 8
  %13 = load i32* %2, align 4
  %14 = sub nsw i32 %13, 1
  %15 = load i32* %3, align 4
  %16 = mul nsw i32 %15, 2
  %17 = add nsw i32 %16, 1
  %18 = call %struct._tree* @generate_tree_rec(i32 %14, i32 %17)
  store %struct._tree* %18, %struct._tree** %right, align 8
  %19 = load i32* %3, align 4
  %20 = load %struct._tree** %left, align 8
  %21 = load %struct._tree** %right, align 8
  %22 = call %struct._tree* @tree_init(i32 %19, %struct._tree* %20, %struct._tree* %21)
  store %struct._tree* %22, %struct._tree** %node, align 8
  %23 = load %struct._tree** %left, align 8
  %24 = load %struct._tree** %node, align 8
  %25 = getelementptr inbounds %struct._tree* %24, i32 0, i32 2
  store %struct._tree* %23, %struct._tree** %25, align 8
  %26 = load %struct._tree** %right, align 8
  %27 = load %struct._tree** %node, align 8
  %28 = getelementptr inbounds %struct._tree* %27, i32 0, i32 3
  store %struct._tree* %26, %struct._tree** %28, align 8
  %29 = load %struct._tree** %node, align 8
  store %struct._tree* %29, %struct._tree** %1
  br label %30

; <label>:30                                      ; preds = %7, %6
  %31 = load %struct._tree** %1
  ret %struct._tree* %31
}

declare void @llvm.gcroot(i8** %ptrloc, i8* %metadata)

define i32 @main() nounwind uwtable gc "jblab-gc" {
  %1 = alloca i32, align 4
  %tr1 = alloca %struct._tree*, align 8
  %tmp = bitcast %struct._tree** %tr1 to i8**   
  call void @llvm.gcroot(i8** %tmp, i8* null)
  store i32 0, i32* %1
  %2 = call %struct._tree* @generate_tree_rec(i32 2, i32 1)
  store %struct._tree* %2, %struct._tree** %tr1, align 8
  %3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8]* @.str4, i32 0, i32 0))
  %4 = call %struct._tree* @generate_tree_rec(i32 3, i32 1)
  store %struct._tree* %4, %struct._tree** %tr1, align 8
  call void (...)* @gc()
  ret i32 0
}

declare void @gc(...)
