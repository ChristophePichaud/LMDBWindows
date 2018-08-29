bool Class::function_xxxx(Params params)
{
	try
	{
		logger.Debug("function_xxxx");

		// code here
	
		return true;
	}
	catch (Exception ex)
	{
		logger.Error("function_xxxx exception...", ex);
		return false;
	}
}
