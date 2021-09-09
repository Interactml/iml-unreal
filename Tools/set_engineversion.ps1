# Script used to update the engine version of a uplugin file
# i.e. this line "EngineVersion": "4.26.0",
# PARAMETERS: <filepath> <ue_version>

if ($args[0] -eq $null) {
	"Expected file path parameter first"
	exit
}
$file = $args[0]
if ($args[1] -eq $null) {
	"Expected version number parameter second"
	exit
}
$ver_param = $args[1]

$var_name = "EngineVersion"
$var_value = $ver_param

echo ("Setting " + $var_name + " to " + $var_value + " in file '" + $file + "'")

$line = Get-Content $file | Select-String $var_name | Select-Object -ExpandProperty Line
if ($line -eq $null) {
	"String Not Found"
	exit
} else {
	$replacement = '	"' + $var_name + '": "' + $var_value + '",'

	$content = Get-Content $file
	$content | ForEach-Object {$_ -replace $line, $replacement} | Set-Content $file
}
