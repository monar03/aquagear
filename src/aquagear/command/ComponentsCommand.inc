<?php 
namespace aquagear\command;

/**
 * コンポーネントコマンドクラス.
 * 
 * このコマンドにはコントロールとビューをあわせた、機能が備わっています。
 * このコマンドを使用する際には、コントロールコマンドとビューコマンドは使用しないでください。
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id$
 *
 */ 
class ComponentsCommand extends CoreCommand {

	/**
	 * テンプレートのベースディレクトリ
	 * 
	 * @var string
	 */
	private $_template_base_dir = null;
	
	/**
	 * グローバルテンプレート
	 * 
	 * @var string
	 */
	private $_global_template = null;
	
	/**
	 * コンポーネントのスタック
	 * 
	 * @var array
	 */
	private $_components_stack = array();
	
	/**
	 * 設定ファイル
	 * 
	 * @var obj
	 */
	private $_conf_obj = null;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 */
	public function init() {
		$xml_template = null;
		$system_conf_arr = \aquagear\application\ApplicationData::getSystemConfigure('component');
		if(isset($system_conf_arr['template_root'])) {
			$this->_template_base_dir = $system_conf_arr['template_root'];
		}
		
		$conf_path = \aquagear\application\ApplicationData::getApplicationConfDir() . "/control.ini"; 
		if(is_file($conf_path)) {
			$this->_conf_obj = \aquagear\holder\ConfigureHolder::load($conf_path);
		}

		$model_type = empty($system_conf_arr['model_type']) ? 'file' : $system_conf_arr['model_type'];
		if($this->_conf_obj instanceof \aquagear\holder\ConfigureHolder) {
			$app_conf_model_type = $this->_conf_obj->getValue('model_type');
			if($app_conf_model_type !== false) {
				$model_type = $app_conf_model_type;
			}
		}
				
		return true;
	}
	
	/**
	 * スタックにあるコンポーネントすべてのテンプレートルートの設定
	 * 
	 * @param string $tempalte_root テンプレートルート
	 * 
	 * @return void
	 */
	public final function setTemplateRoot($template_root) {
		$this->_template_base_dir = $template_root;
		foreach ($this->_components_stack as $name) {
			foreach($name as $component) {
				$component->setTemplateRoot($template_root);
			}
		}
	}
	
	/**
	 * コンポーネントXMLから、スタックの作成
	 * 
	 * @param SimpleXMLElement $xml_template コンポーネントXMLのドキュメントルート
	 * @return bool
	 */
	private function _initComponentsStack($xml_template) {
		foreach($xml_template->children() as $component) {
			$id = (string) $component['id'];
			$class_name = (string) $component->class;
			$name = (string) $component->name;
			$cache = array(
						'set'  => (string) $component->cache->set,
						'ttl'  => (string) $component->cache->ttl,
						'type' => (string) $component->cache->type
					);
			$conf_arr = array();
			
			if(isset($component->value)) {
				foreach($component->value->children() as $value) {
					$component_name = $value->getName();
					$conf_arr[$component_name] = (string) $value; 
				}
			}
			
			$tmp_component = new $class_name;
			$template_path = (string) $component->template;
			$tmp_component->init($id, $cache, $this->_template_base_dir, $template_path, $conf_arr);

			$this->_components_stack[$name][$id] = $tmp_component;
		}
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 */
	public function execute() {
		$component_value = array();
		
		//TODO 暫定処理
		if($model_type === 'pdo') {
			$model = \aquagear\model\Model::getModelInstance('pdo')->getValue($this->_conf_obj->getValue('component_design_source'),
																						array('id' => $this->_conf_obj->getValue('id')));

			$xml_template = \aquagear\model\XmlModelConnector::loadXmlFromString($model[0]['design_xml']);
		}
		else {
			$template_path = \aquagear\application\ApplicationData::getAquagearServerEnv('template_root') . "/" . \aquagear\holder\HttpParameterHolder::getInstance()->getURI() . ".design";
			if(! is_file($template_path)) {
				throw new \aquagear\exception\CommandException('cannot components define template file. [template_path='.$template_path.']', '404');
			}
			
			$xml_template = \aquagear\model\XmlModelConnector::loadXmlFromFile($template_path);
		}
		
		$this->_global_template = (string) $xml_template['global_template'];
		$this->_initComponentsStack($xml_template);
		if(empty($this->_components_stack)) {
			throw new \aquagear\exception\CommandException('cannot initialize ComponentsCommand command.', '404');
		}
		//-------------------		
		
		if(! empty($this->_components_stack)) {
			foreach($this->_components_stack as $key => $components) {
				$component_value[$key] = $this->_componentsExecute($components);
			}
		}
		
		if(empty($component_value)) {
			return false;
		}
		
		$this->_render($component_value);
		return true;
	}
	
	/**
	 * コンポーネント群の実行.
	 * 
	 * @param array $components コンポーネント群
	 * 
	 * @return mixed データの返り値。配列もしくは文字列
	 */
	private function _componentsExecute(array $components) {
		$component_value = array();
		
		foreach($components as $key => $component) {
			\aquagear\log\Logger::getLogInstance()->info('execute component [' . get_class($component) . ']');
			
			$component_value[$key] = $component->execute();			
			\aquagear\log\Logger::getLogInstance()->info('end component [' . get_class($component) . ']');
		}

		return $component_value;
	}
	
	/**
	 * render method.
	 * 
	 * @param array $value ホルダー
	 * @return void
	 */
	private function _render(array $value) {
		\aquagear\log\Logger::getLogInstance()->debug('render components template.[' . var_export($value, true) . ']');
		if(!empty($value)){
			extract($value);
		}

		ob_start();
		ob_implicit_flush(0);
		include($this->_template_base_dir . '/' . $this->_global_template);
		$content = ob_get_clean();

		echo $content;
	}
}
?>