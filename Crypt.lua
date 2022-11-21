
local Encode = {
	['a']="a>k", 			['b']="77Bhh", 		['c']="2{sl", 
	['d']="SX(*B?", 		['p']="S((", 		['r']=">Ss.dk", 
	['i']="SD(HB@N@N88s99a", 	['n']="@*b3bhs",	['t']="]2][s][**",
	['h']="218bS1bbLSO2", 		['e']="S*&b2bb278S",	['l']="subsu",
	['o']="??S>28nb(MwmlaIsO", 	[' ']="9SB99([{}++",	['(']="(*Hb123bvuas",
	[')']="))1230897bas0d98???",	['\"']="}}S{22",	['w']="55555"		
}

local function Encrypt(str)
  return string.gsub(string.lower(str), "(.)", function(Char) return Encode[Char] end)	-- Encryption  
end

return Encrypt
