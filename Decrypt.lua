local Decode = {
  ["a>k"]='a', 					["77Bhh"]='b', 		["2{sl"]='c',
	["SX(*B?"]='d', 				["S(("]='p', 		[">Ss.dk"]='r', 
	["SD(HB@N@N88s99a"]='i', 		["@*b3bhs"]='n',	["]2][s][**"]='t',
	["218bS1bbLSO2"]='h', 			["S*&b2bb278S"]='e',["subsu"]='l',
	["??S>28nb(MwmlaIsO"]='o', 		["9SB99([{}++"]=' ',["(*Hb123bvuas"]='(',
	["))1230897bas0d98???"]=')',	["}}S{22"]='\"',	["55555"]='w'
}

local function Decrypt(Data)
	local TempString,Decoded = '',''
	for Char in string.gmatch(Data, "(.)") do
		TempString = TempString..Char
		if Decode[TempString] then
			Decoded = Decoded..Decode[TempString]
			TempString = ''
		else
			continue
		end
	end
	return Decoded
end

return Decrypt
