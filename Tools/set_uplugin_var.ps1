# Script used to update .uplugin values
# PARAMETERS: <filepath> <parameter> <value>

if ($args[0] -eq $null) {
	"Expected .uplugin file path parameter first"
	exit
}
$file = $args[0]
if ($args[1] -eq $null) {
	"Expected name parameter second"
	exit
}
$var_name = '"' + $args[1] + '"'
if ($args[2] -eq $null) {
	"Expected value parameter third"
	exit
}
$var_value = $args[2]

echo ("Setting " + $var_name + " to " + $var_value + " in file '" + $file + "'")

$line = Get-Content $file | Select-String $var_name | Select-Object -ExpandProperty Line

if ($line -match '",$') {
	$var_value = '"' + $var_value + '"'
}

if ($line -eq $null) {
	"String Not Found"
	exit
} else {
	$replacement = '	' + $var_name + ': ' + $var_value + ','

	$content = Get-Content $file
	$content | ForEach-Object {$_ -replace $line, $replacement} | Set-Content $file
}
