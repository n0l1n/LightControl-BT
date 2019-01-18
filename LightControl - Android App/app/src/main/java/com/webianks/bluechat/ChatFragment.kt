package com.webianks.bluechat

import android.content.Context
import android.os.Bundle
import com.google.android.material.floatingactionbutton.FloatingActionButton
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.RecyclerView
import android.text.TextWatcher
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.text.Editable

import com.webianks.bluechat.R

import me.priyesh.chroma.ChromaDialog;
import me.priyesh.chroma.ColorMode;
import me.priyesh.chroma.ColorSelectListener;
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.GridLayoutManager
import android.widget.*

import com.webianks.bluechat.ModeButton
import com.webianks.bluechat.list.ModeAdapter


/**
 * Created by ramankit on 24/7/17.
 */

class ChatFragment : Fragment(), View.OnClickListener {

    private lateinit var chatInput: EditText
    private lateinit var sendButton: FrameLayout
    private lateinit var fab: FloatingActionButton
    private lateinit var mColorModeSpinner: Spinner
    private var communicationListener: CommunicationListener? = null




    private val KEY_COLOR = "extra_color"
    private val KEY_COLOR_MODE = "extra_color_mode"

    private var mColor: Int = 0
   private var bw_progress :Int =0
    private var speed_progress: Int =0
    private var brightness_progress: Int =0

    var Modes =  ArrayList <ModeButton>()

    companion object {
        fun newInstance(): ChatFragment {
            val myFragment = ChatFragment()
            val args = Bundle()
            myFragment.arguments = args
            return myFragment
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)


    }

    override fun onCreateView(
            inflater: LayoutInflater,
            container: ViewGroup?,
            savedInstanceState: Bundle?
    ): View? {

        val mView: View  = LayoutInflater.from(activity).inflate(R.layout.chat_fragment, container, false)
        initViews(mView,savedInstanceState)
        return mView
    }

    private fun initViews(mView: View, savedInstanceState: Bundle?) {


        chatInput = mView.findViewById(R.id.chatInput)
        val chatIcon: ImageView = mView.findViewById(R.id.sendIcon)
        sendButton = mView.findViewById(R.id.sendButton)

        updateTextView()

        val list  = mView.findViewById <ListView>(R.id.modelist)
        val modes = getModes(Modes)
        val prodAdapter = ModeAdapter (modes, this.context!!)
        list.adapter = prodAdapter
        list.dividerHeight = 10

        list.onItemClickListener = AdapterView.OnItemClickListener {
            adapterView, view, i, l ->
            Toast.makeText(this.context!!,
                    "you selected attraction " + (i ),
                    Toast.LENGTH_LONG).show()
            saveData("mode",(i))
            updateTextView()
        }

        fab = mView.findViewById(R.id.fab)
        fab.setOnClickListener{showColorPickerDialog()}
        mColorModeSpinner = mView.findViewById(R.id.color_mode_spinner)

        sendButton.isClickable = false
        sendButton.isEnabled = false

//
        val seekbar_speed = mView.findViewById(R.id.seekBar_speed) as SeekBar
        seekbar_speed.max = 255
        seekbar_speed.progress = speed_progress
        seekbar_speed.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onStartTrackingTouch(seekbar: SeekBar?) { }

            override fun onStopTrackingTouch(seekbar: SeekBar?) {
                saveData("speed",seekbar_speed.progress)
                Toast.makeText(this@ChatFragment.context,
                        "you selected speed:  " + seekbar_speed.progress,
                        Toast.LENGTH_LONG).show()
                updateTextView()
            }

            override fun onProgressChanged(seekbar: SeekBar?, progress: Int, fromUser: Boolean) {

            }
        })
        val seekbar_bw = mView.findViewById(R.id.seekBar_BW) as SeekBar
        seekbar_bw.max = 255
        seekbar_bw.progress = bw_progress
        seekbar_bw.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onStartTrackingTouch(seekbar: SeekBar?) { }

            override fun onStopTrackingTouch(seekbar: SeekBar?) {
                saveData("w",seekbar_bw.progress)
                Toast.makeText(this@ChatFragment.context,
                        "you selected  " + seekbar_bw.progress,
                        Toast.LENGTH_LONG).show()
                updateTextView()
            }
            override fun onProgressChanged(seekbar: SeekBar?, progress: Int, fromUser: Boolean) {

            }
        })


        val seekbar_br = mView.findViewById(R.id.seekBar_BR) as SeekBar
        seekbar_br.max = 255
        seekbar_br.progress = brightness_progress
        seekbar_br.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onStartTrackingTouch(seekbar: SeekBar?) { }

            override fun onStopTrackingTouch(seekbar: SeekBar?) {
                saveData("br",seekbar_br.progress)
                Toast.makeText(this@ChatFragment.context,
                        "you selected BRIGHTNESS " + seekbar_br.progress,
                        Toast.LENGTH_LONG).show()
                updateTextView()
            }
            override fun onProgressChanged(seekbar: SeekBar?, progress: Int, fromUser: Boolean) {

            }
        })

//


        chatInput.addTextChangedListener(object : TextWatcher {
            override fun onTextChanged(s: CharSequence, start: Int, before: Int, count: Int) {}
            override fun beforeTextChanged(s: CharSequence, start: Int, count: Int, after: Int) {}
            override fun afterTextChanged(s: Editable) {

                if (s.isNotEmpty()) {
                    chatIcon.setImageDrawable(activity?.getDrawable(R.drawable.ic_send))
                    sendButton.isClickable = true
                    sendButton.isEnabled = true
                }else {
                    chatIcon.setImageDrawable(activity?.getDrawable(R.drawable.ic_send_depri))
                    sendButton.isClickable = false
                    sendButton.isEnabled = false
                }
            }
        })



        sendButton.setOnClickListener(this)

        mColor = if (savedInstanceState != null)
            savedInstanceState.getInt(KEY_COLOR)
        else
            ContextCompat.getColor(this.context!!, R.color.colorPrimary)

        val colorMode = if (savedInstanceState != null)
            ColorMode.valueOf(savedInstanceState.getString(KEY_COLOR_MODE)!!)
        else
            ColorMode.RGB

        val adapter = ArrayAdapter(
                this.context, android.R.layout.simple_spinner_dropdown_item, ColorMode.values())

        mColorModeSpinner.adapter = adapter
        mColorModeSpinner.setSelection(adapter.getPosition(colorMode))

       updateTextView()

    }

    override fun onClick(p0: View?) {

        if (chatInput.text.isNotEmpty()){
            communicationListener?.onCommunication(chatInput.text.toString())
            chatInput.setText("")
        }

    }


    fun setCommunicationListener(communicationListener: CommunicationListener){
       this.communicationListener = communicationListener
   }

    interface CommunicationListener{
        fun onCommunication(message: String)
    }

    override fun onSaveInstanceState(outState: Bundle) {
        outState.putInt(KEY_COLOR, mColor)
        outState.putString(KEY_COLOR_MODE, mColorModeSpinner.getSelectedItem().toString())
        super.onSaveInstanceState(outState)
    }

    private fun showColorPickerDialog() {

        ChromaDialog.Builder()
                .initialColor(mColor)
                .colorMode(mColorModeSpinner.getSelectedItem() as ColorMode)
                .onColorSelected(object : ColorSelectListener {
                    override fun onColorSelected(color: Int) {
                        saveData("rgb",color)
                        updateTextView()
                        mColor = color
                    }
                })
                .create()
                .show(getActivity()!!.getSupportFragmentManager(), "dialog")
    }

    private fun updateTextView() {
        val sharedPref = this.activity!!.getPreferences(Context.MODE_PRIVATE) ?: return
        val gmode = sharedPref.getInt("mode", 0).toByte()
        val g6mode= String.format("%02X",gmode)
        val grgb = sharedPref.getInt("rgb", 0)
        val g6rgb = String.format("%06X", 0xFFFFFF and grgb)
        val gw = sharedPref.getInt("w", 0).toByte()
        val g6w = String.format("%02X", gw)
        val gspeed = sharedPref.getInt("speed", 0)
        val g6speed = String.format("%02X",gspeed)
        val gbr = sharedPref.getInt("br", 0).toByte()
        val g6br = String.format("%02X", gbr)

        bw_progress = sharedPref.getInt("w", 0)
        speed_progress = sharedPref.getInt("speed", 0)
        brightness_progress = sharedPref.getInt("br", 0)

        chatInput.setText("["+g6mode+":"+g6w+g6rgb+":"+g6br+":"+g6speed+"]")
        communicationListener?.onCommunication(chatInput.text.toString())
    }



    //saving data
    fun saveData( ident: String, value :Any) {
        val sharedPref = this.activity!!.getPreferences(Context.MODE_PRIVATE) ?: return
        with(sharedPref.edit()) {
            when (ident){
                "mode" ->putInt("mode", value as Int)
                "rgb"->putInt("rgb", value as Int)
                "w"->putInt("w", value as Int)
                "speed"->putInt("speed", value as Int)
                "br"->putInt("br", value as Int)
            }
            commit()
        }
    }




    fun getModes(Modes: ArrayList<ModeButton>):ArrayList<ModeButton> {
        val ModeButtonLst = ArrayList<ModeButton>()

        var amodeButton = ModeButton(modeName = "Static",modeid = 0)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Blink",modeid =1)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Breath",modeid =2)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Wipe",modeid =3)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Wipe Inverse",modeid =4)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Wipe Reverse",modeid =5)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Wipe Reverse Inverse",modeid =6)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Wipe Random",modeid =7)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Random Color",modeid =8)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Single Dynamic",modeid =9)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Multi Dynamic",modeid =10)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Rainbow",modeid =11)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Rainbow Cycle",modeid =12)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Scan",modeid =13)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Dual Scan",modeid =14)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fade",modeid =15)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Theater Chase",modeid =16)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Theater Chase Rainbow",modeid =17)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Running Lights",modeid =18)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Twinkle",modeid =19)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Twinkle Random",modeid =20)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Twinkle Fade",modeid =21)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Twinkle Fade Random",modeid =22)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Sparkle",modeid =23)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Flash Sparkle",modeid =24)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Hyper Sparkle",modeid =25)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Strobe",modeid =26)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Strobe Rainbow",modeid =27)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Multi Strobe",modeid =28)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Blink Rainbow",modeid =29)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase White",modeid =30)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Color",modeid =31)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Random",modeid =32)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Rainbow",modeid =33)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Flash",modeid =34)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Flash Random",modeid =35)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Rainbow White",modeid =36)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Blackout",modeid =37)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Chase Blackout Rainbow",modeid =38)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Color Sweep Random",modeid =39)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Running Color",modeid =40)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Running Red Blue",modeid =41)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Running Random",modeid =42)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Larson Scanner",modeid =43)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Comet",modeid =44)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fireworks",modeid =45)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fireworks Random",modeid =46)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Merry Christmas",modeid =47)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fire Flicker",modeid =48)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fire Flicker (soft)",modeid =49)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Fire Flicker (intense)",modeid =50)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Circus Combustus",modeid =51)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Halloween",modeid =52)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Bicolor Chase",modeid =53)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="Tricolor Chase",modeid =54)
        ModeButtonLst.add(amodeButton)
        amodeButton = ModeButton(modeName ="ICU",modeid =55)
        ModeButtonLst.add(amodeButton)

        return ModeButtonLst
    }


}