
var oEditor		= window.parent.InnerDialogLoaded() ;
var TD			= oEditor.TD ;
var TDLang		= oEditor.TDLang ;
var TDConfig	= oEditor.TDConfig ;

//#### Dialog Tabs

// Set the dialog tabs.
window.parent.AddTab( 'Info', oEditor.TDLang.DlgInfoTab ) ;

if ( TDConfig.FlashUpload )
	window.parent.AddTab( 'Upload', TDLang.DlgLnkUpload ) ;

if ( !TDConfig.FlashDlgHideAdvanced )
	window.parent.AddTab( 'Advanced', oEditor.TDLang.DlgAdvancedTag ) ;

// Function called when a dialog tag is selected.
function OnDialogTabChange( tabCode )
{
	ShowE('divInfo'		, ( tabCode == 'Info' ) ) ;
	ShowE('divUpload'	, ( tabCode == 'Upload' ) ) ;
	ShowE('divAdvanced'	, ( tabCode == 'Advanced' ) ) ;
}

// Get the selected flash embed (if available).
var oFakeImage = TD.Selection.GetSelectedElement() ;
var oEmbed ;

if ( oFakeImage )
{
	if ( oFakeImage.tagName == 'IMG' && oFakeImage.getAttribute('_flash') )
		oEmbed = TD.GetRealElement( oFakeImage ) ;
	else
		oFakeImage = null ;
}

window.onload = function()
{
	// Translate the dialog box texts.
	oEditor.TDLanguageManager.TranslatePage(document) ;

	// Load the selected element information (if any).
	LoadSelection() ;

	// Show/Hide the "Browse Server" button.
	GetE('tdBrowse').style.display = TDConfig.FlashBrowser	? '' : 'none' ;

	// Set the actual uploader URL.
	if ( TDConfig.FlashUpload )
		GetE('frmUpload').action = TDConfig.FlashUploadURL ;

	window.parent.SetAutoSize( true ) ;

	// Activate the "OK" button.
	window.parent.SetOkButton( true ) ;
}

function LoadSelection()
{
	if ( ! oEmbed ) return ;

	GetE('txtUrl').value    = GetAttribute( oEmbed, 'src', '' ) ;
	GetE('txtWidth').value  = GetAttribute( oEmbed, 'width', '' ) ;
	GetE('txtHeight').value = GetAttribute( oEmbed, 'height', '' ) ;

	// Get Advances Attributes
	GetE('txtAttId').value		= oEmbed.id ;
	GetE('chkAutoPlay').checked	= GetAttribute( oEmbed, 'play', 'true' ) == 'true' ;
	GetE('chkLoop').checked		= GetAttribute( oEmbed, 'loop', 'true' ) == 'true' ;
	GetE('chkMenu').checked		= GetAttribute( oEmbed, 'menu', 'true' ) == 'true' ;
	GetE('cmbScale').value		= GetAttribute( oEmbed, 'scale', '' ).toLowerCase() ;

	GetE('txtAttTitle').value		= oEmbed.title ;

	if ( oEditor.TDBrowserInfo.IsIE )
	{
		GetE('txtAttClasses').value = oEmbed.getAttribute('className') || '' ;
		GetE('txtAttStyle').value = oEmbed.style.cssText ;
	}
	else
	{
		GetE('txtAttClasses').value = oEmbed.getAttribute('class',2) || '' ;
		GetE('txtAttStyle').value = oEmbed.getAttribute('style',2) || '' ;
	}

	UpdatePreview() ;
}

//#### The OK button was hit.
function Ok()
{
	if ( GetE('txtUrl').value.length == 0 )
	{
		window.parent.SetSelectedTab( 'Info' ) ;
		GetE('txtUrl').focus() ;

		alert( oEditor.TDLang.DlgAlertUrl ) ;

		return false ;
	}

	oEditor.TDUndo.SaveUndoStep() ;
	if ( !oEmbed )
	{
		oEmbed		= TD.EditorDocument.createElement( 'EMBED' ) ;
		oFakeImage  = null ;
	}
	UpdateEmbed( oEmbed ) ;

	if ( !oFakeImage )
	{
		oFakeImage	= oEditor.TDDocumentProcessor_CreateFakeImage( 'TD__Flash', oEmbed ) ;
		oFakeImage.setAttribute( '_flash', 'true', 0 ) ;
		oFakeImage	= TD.InsertElement( oFakeImage ) ;
	}

	oEditor.TDFlashProcessor.RefreshView( oFakeImage, oEmbed ) ;

	return true ;
}

function UpdateEmbed( e )
{
	SetAttribute( e, 'type'			, 'application/x-shockwave-flash' ) ;
	SetAttribute( e, 'pluginspage'	, 'http://www.macromedia.com/go/getflashplayer' ) ;

	SetAttribute( e, 'src', GetE('txtUrl').value ) ;
	SetAttribute( e, "width" , GetE('txtWidth').value ) ;
	SetAttribute( e, "height", GetE('txtHeight').value ) ;

	// Advances Attributes

	SetAttribute( e, 'id'	, GetE('txtAttId').value ) ;
	SetAttribute( e, 'scale', GetE('cmbScale').value ) ;

	SetAttribute( e, 'play', GetE('chkAutoPlay').checked ? 'true' : 'false' ) ;
	SetAttribute( e, 'loop', GetE('chkLoop').checked ? 'true' : 'false' ) ;
	SetAttribute( e, 'menu', GetE('chkMenu').checked ? 'true' : 'false' ) ;

	SetAttribute( e, 'title'	, GetE('txtAttTitle').value ) ;

	if ( oEditor.TDBrowserInfo.IsIE )
	{
		SetAttribute( e, 'className', GetE('txtAttClasses').value ) ;
		e.style.cssText = GetE('txtAttStyle').value ;
	}
	else
	{
		SetAttribute( e, 'class', GetE('txtAttClasses').value ) ;
		SetAttribute( e, 'style', GetE('txtAttStyle').value ) ;
	}
}

var ePreview ;

function SetPreviewElement( previewEl )
{
	ePreview = previewEl ;

	if ( GetE('txtUrl').value.length > 0 )
		UpdatePreview() ;
}

function UpdatePreview()
{
	if ( !ePreview )
		return ;

	while ( ePreview.firstChild )
		ePreview.removeChild( ePreview.firstChild ) ;

	if ( GetE('txtUrl').value.length == 0 )
		ePreview.innerHTML = '&nbsp;' ;
	else
	{
		var oDoc	= ePreview.ownerDocument || ePreview.document ;
		var e		= oDoc.createElement( 'EMBED' ) ;

		SetAttribute( e, 'src', GetE('txtUrl').value ) ;
		SetAttribute( e, 'type', 'application/x-shockwave-flash' ) ;
		SetAttribute( e, 'width', '100%' ) ;
		SetAttribute( e, 'height', '100%' ) ;

		ePreview.appendChild( e ) ;
	}
}

// <embed id="ePreview" src="flash/claims.swf" width="100%" height="100%" style="visibility:hidden" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer">

function BrowseServer()
{
	OpenFileBrowser( TDConfig.FlashBrowserURL, TDConfig.FlashBrowserWindowWidth, TDConfig.FlashBrowserWindowHeight ) ;
}

function SetUrl( url, width, height )
{
	GetE('txtUrl').value = url ;

	if ( width )
		GetE('txtWidth').value = width ;

	if ( height )
		GetE('txtHeight').value = height ;

	UpdatePreview() ;

	window.parent.SetSelectedTab( 'Info' ) ;
}

function OnUploadCompleted( errorNumber, fileUrl, fileName, customMsg )
{
	switch ( errorNumber )
	{
		case 0 :	// No errors
			alert( 'Your file has been successfully uploaded' ) ;
			break ;
		case 1 :	// Custom error
			alert( customMsg ) ;
			return ;
		case 101 :	// Custom warning
			alert( customMsg ) ;
			break ;
		case 201 :
			alert( 'A file with the same name is already available. The uploaded file has been renamed to "' + fileName + '"' ) ;
			break ;
		case 202 :
			alert( 'Invalid file type' ) ;
			return ;
		case 203 :
			alert( "Security error. You probably don't have enough permissions to upload. Please check your server." ) ;
			return ;
		default :
			alert( 'Error on file upload. Error number: ' + errorNumber ) ;
			return ;
	}

	SetUrl( fileUrl ) ;
	GetE('frmUpload').reset() ;
}

var oUploadAllowedExtRegex	= new RegExp( TDConfig.FlashUploadAllowedExtensions, 'i' ) ;
var oUploadDeniedExtRegex	= new RegExp( TDConfig.FlashUploadDeniedExtensions, 'i' ) ;

function CheckUpload()
{
	var sFile = GetE('txtUploadFile').value ;

	if ( sFile.length == 0 )
	{
		alert( 'Please select a file to upload' ) ;
		return false ;
	}

	if ( ( TDConfig.FlashUploadAllowedExtensions.length > 0 && !oUploadAllowedExtRegex.test( sFile ) ) ||
		( TDConfig.FlashUploadDeniedExtensions.length > 0 && oUploadDeniedExtRegex.test( sFile ) ) )
	{
		OnUploadCompleted( 202 ) ;
		return false ;
	}

	return true ;
}