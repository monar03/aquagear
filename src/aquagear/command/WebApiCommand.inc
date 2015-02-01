<?php 
namespace aquagear\command;

/**
 * WebApi出力用コマンド
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id$
 */ 
class WebApiCommand extends CoreCommand {	
	/**
	 * 設定ファイルオブジェクト
	 * 
	 * @var \aquagear\holder\ConfigureHolder
	 */
	private $_conf_obj = null;
	
	/**
	 * コントローラオブジェクト
	 * 
	 * @var obj
	 */
	private $_control_obj = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 * 
	 * @FIXME test
	 */
	public function init() {
		$conf_path = \aquagear\application\ApplicationData::getApplicationConfDir() . '/api.ini';
		if(is_file($conf_path)) {
			$this->_conf_obj = \aquagear\holder\ConfigureHolder::load($conf_path);
		}
		
		$class_name = $this->getClassName();
		if(! class_exists($class_name)) {
			\aquagear\log\Logger::getLogInstance()->debug('cannot exists class [ $class_name = ' . $class_name . ' ]');
			return false;
		}
			
		\aquagear\log\Logger::getLogInstance()->debug("execute control [class_name = $class_name]");
		$this->_control_obj = new $class_name;
		
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 * 	 
	 * @FIXME test
	 */
	public function execute() {
		$class_name = "";
		$data_arr = array();
		
		if($this->_control_obj instanceof \aquagear\control\CoreWebApiControl) {
			$this->_control_obj->init($this->_conf_obj);
			$data_arr = $this->_control_obj->execute();
		}
		else {
			throw new \aquagear\exception\CommandException('not ControlObject.', '404');
		}
		
		$this->_view($data_arr);
		return true;		
	}

	/**
	 * 表示
	 * 
	 * @param array $data_arr データ
	 * 
	 * @return bool 成功時true、失敗時false
	 */
	private function _view(array $data_arr) {
		if(\aquagear\holder\HttpParameterHolder::getInstance()->getGetValue('format') === 'json' &&
			function_exists('json_encode')) {
			$this->_renderJson($data_arr);
		}
		else if(\aquagear\holder\HttpParameterHolder::getInstance()->getGetValue('format') === 'sphp') {
			$this->_renderSphp($data_arr);
		}
		else {
			$this->_renderXml($data_arr);
		}
		
		return true;
	}
	
	/**
	 * json形式での表示
	 * 
	 * @param array $data_arr データ
	 * 
	 * @return void
	 * 
	 * @since php5.2
	 */
	private function _renderJson(array $data_arr) {
		echo json_encode($data_arr);
	}
	
	/**
	 * serialize形式での表示
	 * 
	 * @param array $data_arr データ
	 * 
	 * @return void
	 * 
	 * @since php5.2
	 */
	private function _renderSphp(array $data_arr) {
		echo serialize($data_arr);
	}
	
	/**
	 * XML形式での表示
	 * 
	 * @param array $data_arr データ
	 * 
	 * @return void
	 */
	private function _renderXml(array $data_arr) {
		$dom_document = new DOMDocument();
		$dom_document->formatOutput = true;
		$dom_document->appendChild($this->_getXML($dom_document, $dom_document->createElement('Response'), $data_arr));
		
		echo $dom_document->saveXML();
	}
	
	/**
	 * XMLノード作成用再起関数
	 * 
	 * @param mixed $data_mixed データ
	 * 
	 * @return DOMElement 追加するDOMElement
	 */
	private function _getXML($dom_document, $dom_element, $data_mixed) {
		if(is_array($data_mixed)) {
			foreach($data_mixed as $key => $value) {
				if(preg_match('/^@/', $key)) {
					$dom_element->appendChild($this->_getXML($dom_document, $dom_document->createAttribute(ltrim($key, '@')), $value));
				}
				else {
					if(is_int($key)) {
						$dom_element = $this->_getXML($dom_document, clone $dom_element, $value);
					}
					else {
						$dom_element->appendChild($this->_getXML($dom_document, $dom_document->createElement($key), $value));
					}
				}
			}
			
			return $dom_element;
		}
		else {
			$dom_element->appendChild($dom_document->createTextNode($data_mixed));
			return $dom_element;
		}
	}
	
	/**
	 * コントローラのクラス名の取得
	 * 
	 * @return string
	 */
	private function getClassName() {
		$uri_arr = explode('/', ltrim(\aquagear\holder\HttpParameterHolder::getInstance()->getURI(), '\/'));
		$uri_arr[count($uri_arr) - 1] = ucwords($uri_arr[count($uri_arr) - 1]);

		return  join('_', $uri_arr) . "Control";
	}
}
?>