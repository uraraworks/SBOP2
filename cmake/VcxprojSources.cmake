# .vcxproj の <ClCompile Include="..."> を読み、CMake のソース一覧に変換する。
#
# ソース一覧の正は各 .vcxproj（docs/cloud-dev-staging-plan.md の S1/S2 方針）。
# CMake 側に一覧を複製しないことで、Visual Studio でファイルを足しても
# Linux ビルドが自動で追従する。
#
# 除外の判定は Release|Win32 構成の <ExcludedFromBuild> に合わせる。
# Linux ビルドは _DEBUG を付けない前提なので、Release と同じく
# DebugFixtureHandler / DebugFixtureGuard などデバッグ専用ファイルは入らない。

# sbo_read_vcxproj_sources(<出力変数> <.vcxprojのパス>)
#   出力は絶対パスのリスト。.vcxproj に書かれたファイルが実在しない場合は
#   （大文字小文字の違いを含め）configure を失敗させる。
function(sbo_read_vcxproj_sources out_var vcxproj)
	get_filename_component(proj_dir "${vcxproj}" DIRECTORY)
	file(STRINGS "${vcxproj}" lines)

	set(result "")
	set(current "")
	set(excluded FALSE)
	foreach(line IN LISTS lines)
		if(line MATCHES "<ClCompile Include=\"([^\"]+)\"")
			set(current "${CMAKE_MATCH_1}")
			set(excluded FALSE)
			# 1行で閉じる <ClCompile Include="..." /> はその場で確定する
			if(line MATCHES "/>[ \t]*$")
				_sbo_append_source(result "${proj_dir}" "${current}" FALSE "${vcxproj}")
				set(current "")
			endif()
		elseif(NOT current STREQUAL "")
			if(line MATCHES "<ExcludedFromBuild Condition=\"[^\"]*Release\\|Win32[^\"]*\">[ \t]*true")
				set(excluded TRUE)
			elseif(line MATCHES "</ClCompile>")
				_sbo_append_source(result "${proj_dir}" "${current}" ${excluded} "${vcxproj}")
				set(current "")
			endif()
		endif()
	endforeach()

	set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

function(_sbo_append_source list_var proj_dir rel excluded vcxproj)
	if(excluded)
		return()
	endif()
	string(REPLACE "\\" "/" rel "${rel}")
	get_filename_component(abs "${proj_dir}/${rel}" ABSOLUTE)
	if(NOT EXISTS "${abs}")
		# Windows は大文字小文字を区別しないため、.vcxproj には "..\common\crc.cpp"
		# のように実ファイルと綴りの違う記述が残っている。1階層ずつ照合して実在の名前に直す。
		_sbo_resolve_case_insensitive(abs "${abs}")
	endif()
	if(NOT EXISTS "${abs}")
		message(FATAL_ERROR "${vcxproj} に書かれたソースが見つかりません: ${rel}")
	endif()
	set(tmp "${${list_var}}")
	list(APPEND tmp "${abs}")
	set(${list_var} "${tmp}" PARENT_SCOPE)
endfunction()

# 絶対パスを先頭から1階層ずつ、大文字小文字を無視して実在の名前に置き換える
function(_sbo_resolve_case_insensitive out_var path)
	string(REGEX MATCHALL "[^/]+" parts "${path}")
	set(cur "")
	foreach(part IN LISTS parts)
		if(EXISTS "${cur}/${part}")
			set(cur "${cur}/${part}")
			continue()
		endif()
		string(TOLOWER "${part}" part_lower)
		file(GLOB children RELATIVE "${cur}/" "${cur}/*")
		set(found "")
		foreach(child IN LISTS children)
			string(TOLOWER "${child}" child_lower)
			if(child_lower STREQUAL part_lower)
				set(found "${child}")
				break()
			endif()
		endforeach()
		if(found STREQUAL "")
			set(${out_var} "${path}" PARENT_SCOPE)
			return()
		endif()
		set(cur "${cur}/${found}")
	endforeach()
	set(${out_var} "${cur}" PARENT_SCOPE)
endfunction()
